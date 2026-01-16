// Copyright Lim Young.


#include "GameplayInputArbiter.h"


bool UGameplayInputDocket::HasInputSourceCommand(const FGameplayTag& InputSourceTag, EGameplayInputType InputType) const
{
	return InputSources.Contains(FGameplayInputSourceCommand(InputSourceTag, InputType));
}

bool UGameplayInputDocket::HasInputActionEvent(const FGameplayTag& InputActionTag,
                                               EGameplayInputActionState ActionState) const
{
	const FGameplayInputActionEvent InputActionEvent(InputActionTag);
	if (!InputActions.Contains(InputActionEvent))
	{
		return false;
	}

	return InputActions[InputActionEvent].ListenedActionStates | static_cast<uint8>(ActionState);
}

FGameplayInputSourceCommandConfig& UGameplayInputDocket::GetInputSourceCommandConfig(const FGameplayTag& InputSourceTag,
	EGameplayInputType InputType)
{
	return InputSources.FindChecked(FGameplayInputSourceCommand(InputSourceTag, InputType));
}

FGameplayInputActionEventConfig& UGameplayInputDocket::GetInputActionEventConfig(const FGameplayTag& InputActionTag)
{
	return InputActions.FindChecked(FGameplayInputActionEvent(InputActionTag));
}

void UGameplayInputArbiter::Initialize(UGameplayInputDocket* InGameplayInputDocker,
                                       const EArbiterCommandMatchMode InMatchMode)
{
	GameplayInputDocker = InGameplayInputDocker;
	MatchMode = InMatchMode;
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
                                                        const TObjectPtr<UGameplayInputSourceCommandInstance>
                                                        CurrentCommand)
{
	// Command expired
	if (CurrentCommand->Lifetime != 0.0f && (CurrentTime - CurrentCommand->Timestamp) > CurrentCommand->
		Lifetime)
	{
		return true;
	}
	return false;
}

bool UGameplayInputArbiter::CheckIfTheEventHasExpired(float CurrentTime,
                                                      TObjectPtr<UGameplayInputActionEventInstance> CurrentEvent)
{
	// Event expired
	if (CurrentEvent->Lifetime != 0.0f && (CurrentTime - CurrentEvent->Timestamp) > CurrentEvent->
		Lifetime)
	{
		return true;
	}
	return false;
}

bool UGameplayInputArbiter::MarchInputSourceCommand(UGameplayInputSourceCommandInstance*& ResultCommand,
                                                    const float CurrentTime)
{
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

bool UGameplayInputArbiter::MarchInputActionEvent(UGameplayInputActionEventInstance*& ResultEvent, float CurrentTime)
{
	switch (GameplayInputDocker->DeliberationMode)
	{
	case EArbiterDeliberationMode::FirstSurvivalCommand:
		{
			for (const TObjectPtr<UGameplayInputActionEventInstance> CurrentEvent : GameplayInputActionEventQueue)
			{
				if (!CurrentEvent)
				{
					continue;
				}

				// Event expired
				if (CheckIfTheEventHasExpired(CurrentTime, CurrentEvent))
				{
					continue;
				}

				ResultEvent = CurrentEvent;
				return true;
			}
			return false;
		}
	case EArbiterDeliberationMode::LastSurvivalCommand:
		{
			for (int32 Index = GameplayInputActionEventQueue.Num() - 1; Index >= 0; --Index)
			{
				const TObjectPtr<UGameplayInputActionEventInstance> CurrentEvent = GameplayInputActionEventQueue[Index];
				if (!CurrentEvent)
				{
					continue;
				}

				// Event expired
				if (CheckIfTheEventHasExpired(CurrentTime, CurrentEvent))
				{
					continue;
				}

				ResultEvent = CurrentEvent;
				return true;
			}

			return false;
		}
	case EArbiterDeliberationMode::FirstCommand:
		{
			if (GameplayInputActionEventQueue.Num() > 0)
			{
				ResultEvent = GameplayInputActionEventQueue[0];
				return true;
			}

			return false;
		}
	case EArbiterDeliberationMode::LastCommand:
		{
			if (GameplayInputActionEventQueue.Num() > 0)
			{
				ResultEvent = GameplayInputActionEventQueue.Last();
				return true;
			}

			return false;
		}
		[[likely]]default:
		{
			TObjectPtr<UGameplayInputActionEventInstance> HighestPriorityEvent = nullptr;
			uint8 HighestPriority = 0;

			for (const TObjectPtr<UGameplayInputActionEventInstance> CurrentEvent : GameplayInputActionEventQueue)
			{
				if (!CurrentEvent)
				{
					continue;
				}

				if (CheckIfTheEventHasExpired(CurrentTime, CurrentEvent))
				{
					continue;
				}

				// Find Priority Command
				if (CurrentEvent->Priority >= HighestPriority)
				{
					HighestPriority = CurrentEvent->Priority;
					HighestPriorityEvent = CurrentEvent;
				}
			}

			if (HighestPriorityEvent)
			{
				ResultEvent = HighestPriorityEvent;
				return true;
			}

			return false;
		}
	}
}

bool UGameplayInputArbiter::Finish(UGameplayInputSourceCommandInstance*& ResultInputSourceCommand,
                                   UGameplayInputActionEventInstance*& ResultInputActionEvent)
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	bool bSuccess = false;

	if (ShouldMatchInputSourceCommand())
	{
		bSuccess |= MarchInputSourceCommand(ResultInputSourceCommand, CurrentTime);
	}

	if (ShouldMatchInputActionEvent())
	{
		bSuccess |= MarchInputActionEvent(ResultInputActionEvent, CurrentTime);
	}

	return bSuccess;
}

bool UGameplayInputArbiter::ReceiveGameplayInputSourceCommand(const FGameplayTag InputSourceTag,
                                                              const EGameplayInputType InputType)
{
	if (ShouldMatchInputSourceCommand())
	{
		if (GameplayInputDocker->HasInputSourceCommand(InputSourceTag, InputType))
		{
			const FGameplayInputSourceCommandConfig InputCommandConfig = GameplayInputDocker->
				GetInputSourceCommandConfig(
					InputSourceTag, InputType);

			UGameplayInputSourceCommandInstance* NewInputCommand = NewObject<UGameplayInputSourceCommandInstance>(this);
			NewInputCommand->Initialize(InputSourceTag, InputType, InputCommandConfig);

			GameplayInputCommandQueue.Add(NewInputCommand);
			return true;
		}
	}
	return false;
}

bool UGameplayInputArbiter::ReceiveGameplayInputActionEvent(const FGameplayTag& InputActionTag,
                                                            const EGameplayInputActionState ActionState)
{
	if (ShouldMatchInputActionEvent())
	{
		if (GameplayInputDocker->HasInputActionEvent(InputActionTag, ActionState))
		{
			const FGameplayInputActionEventConfig InputActionConfig = GameplayInputDocker->
				GetInputActionEventConfig(InputActionTag);

			UGameplayInputActionEventInstance* NewInputActionEvent = NewObject<UGameplayInputActionEventInstance>(this);
			NewInputActionEvent->Initialize(InputActionTag, ActionState, InputActionConfig);

			GameplayInputActionEventQueue.Add(NewInputActionEvent);
			return true;
		}
	}
	return false;
}

EArbiterCommandMatchMode UGameplayInputArbiter::GetMatchMode() const
{
	return MatchMode;
}

bool UGameplayInputArbiter::ShouldMatchInputSourceCommand() const
{
	return MatchMode == EArbiterCommandMatchMode::InputSourceOnly ||
		MatchMode == EArbiterCommandMatchMode::Both;
}

bool UGameplayInputArbiter::ShouldMatchInputActionEvent() const
{
	return MatchMode == EArbiterCommandMatchMode::InputActionOnly ||
		MatchMode == EArbiterCommandMatchMode::Both;
}
