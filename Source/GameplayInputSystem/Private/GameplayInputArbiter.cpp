// Copyright Lim Young.


#include "GameplayInputArbiter.h"


bool UGameplayInputDocket::HasCommand(const FGameplayTag& InputTag, EGameplayInputType InputType) const
{
	return InputCommands.Contains(FGameplayInputCommand(InputTag, InputType));
}

FGameplayInputCommandConfig& UGameplayInputDocket::GetCommandConfig(const FGameplayTag& InputTag,
                                                                    EGameplayInputType InputType)
{
	return InputCommands.FindChecked(FGameplayInputCommand(InputTag, InputType));
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
                                                        const TObjectPtr<UGameplayInputCommandInstance> CurrentCommand)
{
	// Command expired
	if (CurrentCommand->Lifetime != 0.0f && (CurrentTime - CurrentCommand->Timestamp) > CurrentCommand->
		Lifetime)
	{
		return true;
	}
	return false;
}

bool UGameplayInputArbiter::Finish(UGameplayInputCommandInstance*& ResultCommand)
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	switch (GameplayInputDocker->DeliberationMode)
	{
	case EArbiterDeliberationMode::FirstSurvivalCommand:
		{
			for (const TObjectPtr<UGameplayInputCommandInstance> CurrentCommand : GameplayInputCommandQueue)
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
				const TObjectPtr<UGameplayInputCommandInstance> CurrentCommand = GameplayInputCommandQueue[Index];
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
			TObjectPtr<UGameplayInputCommandInstance> HighestPriorityCommand = nullptr;
			uint8 HighestPriority = 0;

			for (const TObjectPtr<UGameplayInputCommandInstance> CurrentCommand : GameplayInputCommandQueue)
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
	if (GameplayInputDocker->HasCommand(InputTag, InputType))
	{
		const FGameplayInputCommandConfig InputCommandConfig = GameplayInputDocker->GetCommandConfig(InputTag, InputType);

		UGameplayInputCommandInstance* NewInputCommand = NewObject<UGameplayInputCommandInstance>(this);
		NewInputCommand->Initialize(InputTag, InputType, InputCommandConfig);

		GameplayInputCommandQueue.Add(NewInputCommand);
		return true;
	}
	return false;
}
