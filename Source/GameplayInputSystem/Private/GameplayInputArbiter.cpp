// Copyright Lim Young.


#include "GameplayInputArbiter.h"

void UGameplayInputCommand::Initialize(const FGameplayTag& InInputTag, const EGameplayInputType InInputType,
                                       uint8 InPriority, float InLifetime, float InTimestamp)
{
	InputTag = InInputTag;
	InputType = InInputType;
	Priority = InPriority;
	Lifetime = InLifetime;

	Timestamp = InTimestamp;
}

void UGameplayInputCommand::Initialize(const FGameplayTag& InInputTag, const EGameplayInputType InInputType,
                                       const FGameplayInputCommandConfig& InConfig)
{
	Initialize(InInputTag, InInputType, InConfig.Priority, InConfig.Lifetime, GetWorld()->GetTimeSeconds());
}

bool UGameplayInputDocket::HasCommandForTag(const FGameplayTag& InputTag) const
{
	return InputCommands.Contains(InputTag);
}

FGameplayInputCommandConfig& UGameplayInputDocket::GetCommandConfigForTag(const FGameplayTag& InputTag)
{
	return InputCommands.FindChecked(InputTag);
}

void UGameplayInputArbiter::Initialize(UGameplayInputDocket* InGameplayInputDocker)
{
	GameplayInputDocker = InGameplayInputDocker;
}

void UGameplayInputArbiter::Start()
{
	GameplayInputCommandQueue.Empty();
}

void UGameplayInputArbiter::Cancel()
{
	GameplayInputCommandQueue.Empty();
}

bool UGameplayInputArbiter::CheckIfTheCommandHasExpired(const float CurrentTime,
                                                        const TObjectPtr<UGameplayInputCommand> CurrentCommand)
{
	// Command expired
	if (CurrentCommand->Lifetime != 0.0f && (CurrentTime - CurrentCommand->Timestamp) > CurrentCommand->
		Lifetime)
	{
		return true;
	}
	return false;
}

bool UGameplayInputArbiter::Finish(UGameplayInputCommand*& ResultCommand)
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	switch (GameplayInputDocker->DeliberationMode)
	{
	case EArbiterDeliberationMode::FirstSurvivalCommand:
		{
			for (const TObjectPtr<UGameplayInputCommand> CurrentCommand : GameplayInputCommandQueue)
			{
				if (!CurrentCommand)
				{
					continue;
				}

				// Command expired
				if (CheckIfTheCommandHasExpired(CurrentTime, CurrentCommand))
				{
					continue;
				}

				ResultCommand = CurrentCommand;
				return true;
			}
			return false;
		}
	case EArbiterDeliberationMode::LastSurvivalCommand:
		{
			for (int32 Index = GameplayInputCommandQueue.Num() - 1; Index >= 0; --Index)
			{
				const TObjectPtr<UGameplayInputCommand> CurrentCommand = GameplayInputCommandQueue[Index];
				if (!CurrentCommand)
				{
					continue;
				}

				// Command expired
				if (CheckIfTheCommandHasExpired(CurrentTime, CurrentCommand))
				{
					continue;
				}

				ResultCommand = CurrentCommand;
				return true;
			}

			return false;
		}
	case EArbiterDeliberationMode::FirstCommand:
		{
			if (GameplayInputCommandQueue.Num() > 0)
			{
				ResultCommand = GameplayInputCommandQueue[0];
				return true;
			}

			return false;
		}
	case EArbiterDeliberationMode::LastCommand:
		{
			if (GameplayInputCommandQueue.Num() > 0)
			{
				ResultCommand = GameplayInputCommandQueue.Last();
				return true;
			}

			return false;
		}
		[[likely]]default:
		{
			TObjectPtr<UGameplayInputCommand> HighestPriorityCommand = nullptr;
			uint8 HighestPriority = 0;

			for (const TObjectPtr<UGameplayInputCommand> CurrentCommand : GameplayInputCommandQueue)
			{
				if (!CurrentCommand)
				{
					continue;
				}

				if (CheckIfTheCommandHasExpired(CurrentTime, CurrentCommand))
				{
					continue;
				}

				// Find Priority Command
				if (CurrentCommand->Priority >= HighestPriority)
				{
					HighestPriority = CurrentCommand->Priority;
					HighestPriorityCommand = CurrentCommand;
				}
			}

			if (HighestPriorityCommand)
			{
				ResultCommand = HighestPriorityCommand;
				return true;
			}

			return false;
		}
	}
}

bool UGameplayInputArbiter::ReceiveGameplayInput(const FGameplayTag InputTag, EGameplayInputType InputType)
{
	if (GameplayInputDocker->HasCommandForTag(InputTag))
	{
		const FGameplayInputCommandConfig InputCommandConfig = GameplayInputDocker->GetCommandConfigForTag(InputTag);

		UGameplayInputCommand* NewInputCommand = NewObject<UGameplayInputCommand>(this);
		NewInputCommand->Initialize(InputTag, InputType, InputCommandConfig);

		GameplayInputCommandQueue.Add(NewInputCommand);
		return true;
	}
	return false;
}
