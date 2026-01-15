// Copyright Lim Young.


#include "GameplayInputArbiter.h"


bool UGameplayInputDocket::HasCommand(const FGameplayTag& InputSourceTag, EGameplayInputType InputType) const
{
	return InputSources.Contains(FGameplayInputSourceCommand(InputSourceTag, InputType));
}

FGameplayInputSourceCommandConfig& UGameplayInputDocket::GetCommandConfig(const FGameplayTag& InputSourceTag,
                                                                    EGameplayInputType InputType)
{
	return InputSources.FindChecked(FGameplayInputSourceCommand(InputSourceTag, InputType));
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
                                                        const TObjectPtr<UGameplayInputSourceCommandInstance> CurrentCommand)
{
	// Command expired
	if (CurrentCommand->Lifetime != 0.0f && (CurrentTime - CurrentCommand->Timestamp) > CurrentCommand->
		Lifetime)
	{
		return true;
	}
	return false;
}

bool UGameplayInputArbiter::Finish(UGameplayInputSourceCommandInstance*& ResultCommand)
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	switch (GameplayInputDocker->DeliberationMode)
	{
	case EArbiterDeliberationMode::FirstSurvivalCommand:
		{
			for (const TObjectPtr<UGameplayInputSourceCommandInstance> CurrentCommand : GameplayInputCommandQueue)
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
				const TObjectPtr<UGameplayInputSourceCommandInstance> CurrentCommand = GameplayInputCommandQueue[Index];
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
			TObjectPtr<UGameplayInputSourceCommandInstance> HighestPriorityCommand = nullptr;
			uint8 HighestPriority = 0;

			for (const TObjectPtr<UGameplayInputSourceCommandInstance> CurrentCommand : GameplayInputCommandQueue)
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

bool UGameplayInputArbiter::ReceiveGameplayInput(const FGameplayTag InputSourceTag, EGameplayInputType InputType)
{
	if (GameplayInputDocker->HasCommand(InputSourceTag, InputType))
	{
		const FGameplayInputSourceCommandConfig InputCommandConfig = GameplayInputDocker->GetCommandConfig(InputSourceTag, InputType);

		UGameplayInputSourceCommandInstance* NewInputCommand = NewObject<UGameplayInputSourceCommandInstance>(this);
		NewInputCommand->Initialize(InputSourceTag, InputType, InputCommandConfig);

		GameplayInputCommandQueue.Add(NewInputCommand);
		return true;
	}
	return false;
}
