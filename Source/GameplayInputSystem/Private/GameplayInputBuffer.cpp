// Copyright Lim Young.


#include "GameplayInputBuffer.h"


bool UGameplayInputBufferSchema::HasInputSourceCommand(const FGameplayTag& InputSourceTag,
                                                       const EGameplayInputType InputType) const
{
	return InputSources.Contains(FGameplayInputSourceCommand(InputSourceTag, InputType));
}

bool UGameplayInputBufferSchema::HasInputActionEvent(const FGameplayTag& InputActionTag,
                                                     EGameplayInputActionEvent ActionEvent) const
{
	const FGameplayInputActionEvent InputActionEvent(InputActionTag);
	if (!InputActions.Contains(InputActionEvent))
	{
		return false;
	}

	return InputActions[InputActionEvent].ListenedActionEvents | static_cast<uint8>(ActionEvent);
}

FGameplayInputSourceCommandConfig& UGameplayInputBufferSchema::GetInputSourceCommandConfig(
	const FGameplayTag& InputSourceTag,
	const EGameplayInputType InputType)
{
	return InputSources.FindChecked(FGameplayInputSourceCommand(InputSourceTag, InputType));
}

FGameplayInputActionEventConfig& UGameplayInputBufferSchema::GetInputActionEventConfig(
	const FGameplayTag& InputActionTag)
{
	return InputActions.FindChecked(FGameplayInputActionEvent(InputActionTag));
}

void UGameplayInputBuffer::Initialize(UGameplayInputBufferSchema* InGameplayInputDocker,
                                      const EGameplayInputBufferScope InBufferScope)
{
	GameplayInputBufferSchema = InGameplayInputDocker;
	BufferScope = InBufferScope;
}

void UGameplayInputBuffer::Start()
{
	GameplayInputSourceCommandQueue.Empty();
}

void UGameplayInputBuffer::Cancel()
{
	GameplayInputSourceCommandQueue.Empty();
}

bool UGameplayInputBuffer::CheckIfTheCommandHasExpired(const float CurrentTime,
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

bool UGameplayInputBuffer::CheckIfTheEventHasExpired(const float CurrentTime,
                                                     const TObjectPtr<UGameplayInputActionEventInstance> CurrentEvent)
{
	// Event expired
	if (CurrentEvent->Lifetime != 0.0f && (CurrentTime - CurrentEvent->Timestamp) > CurrentEvent->
		Lifetime)
	{
		return true;
	}
	return false;
}

bool UGameplayInputBuffer::BuffInputSourceCommand(UGameplayInputSourceCommandInstance*& ResultCommand,
                                                  const float CurrentTime)
{
	switch (GameplayInputBufferSchema->InputBufferPolicy)
	{
	case EGameplayInputBufferPolicy::FirstSurvivalCommand:
		{
			for (const TObjectPtr<UGameplayInputSourceCommandInstance> CurrentCommand : GameplayInputSourceCommandQueue)
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
	case EGameplayInputBufferPolicy::LastSurvivalCommand:
		{
			for (int32 Index = GameplayInputSourceCommandQueue.Num() - 1; Index >= 0; --Index)
			{
				const TObjectPtr<UGameplayInputSourceCommandInstance> CurrentCommand = GameplayInputSourceCommandQueue[
					Index];
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
	case EGameplayInputBufferPolicy::FirstCommand:
		{
			if (GameplayInputSourceCommandQueue.Num() > 0)
			{
				ResultCommand = GameplayInputSourceCommandQueue[0];
				return true;
			}

			return false;
		}
	case EGameplayInputBufferPolicy::LastCommand:
		{
			if (GameplayInputSourceCommandQueue.Num() > 0)
			{
				ResultCommand = GameplayInputSourceCommandQueue.Last();
				return true;
			}

			return false;
		}
		[[likely]]default:
		{
			TObjectPtr<UGameplayInputSourceCommandInstance> HighestPriorityCommand = nullptr;
			uint8 HighestPriority = 0;

			for (const TObjectPtr<UGameplayInputSourceCommandInstance> CurrentCommand : GameplayInputSourceCommandQueue)
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

bool UGameplayInputBuffer::BuffInputActionEvent(UGameplayInputActionEventInstance*& ResultEvent,
                                                const float CurrentTime)
{
	switch (GameplayInputBufferSchema->InputBufferPolicy)
	{
	case EGameplayInputBufferPolicy::FirstSurvivalCommand:
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
	case EGameplayInputBufferPolicy::LastSurvivalCommand:
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
	case EGameplayInputBufferPolicy::FirstCommand:
		{
			if (GameplayInputActionEventQueue.Num() > 0)
			{
				ResultEvent = GameplayInputActionEventQueue[0];
				return true;
			}

			return false;
		}
	case EGameplayInputBufferPolicy::LastCommand:
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

bool UGameplayInputBuffer::Finish(UGameplayInputSourceCommandInstance*& ResultInputSourceCommand,
                                  UGameplayInputActionEventInstance*& ResultInputActionEvent)
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	bool bSuccess = false;

	if (ShouldBuffInputSourceCommand())
	{
		bSuccess |= BuffInputSourceCommand(ResultInputSourceCommand, CurrentTime);
	}

	if (ShouldBuffInputActionEvent())
	{
		bSuccess |= BuffInputActionEvent(ResultInputActionEvent, CurrentTime);
	}

	return bSuccess;
}

bool UGameplayInputBuffer::ReceiveGameplayInputSourceCommand(const FGameplayTag InputSourceTag,
                                                             const EGameplayInputType InputType)
{
	if (ShouldBuffInputSourceCommand())
	{
		if (GameplayInputBufferSchema->HasInputSourceCommand(InputSourceTag, InputType))
		{
			const FGameplayInputSourceCommandConfig InputCommandConfig = GameplayInputBufferSchema->
				GetInputSourceCommandConfig(
					InputSourceTag, InputType);

			UGameplayInputSourceCommandInstance* NewInputCommand = NewObject<UGameplayInputSourceCommandInstance>(this);
			NewInputCommand->Initialize(InputSourceTag, InputType, InputCommandConfig);

			GameplayInputSourceCommandQueue.Add(NewInputCommand);
			return true;
		}
	}
	return false;
}

bool UGameplayInputBuffer::ReceiveGameplayInputActionEvent(const FGameplayTag& InputActionTag,
                                                           EGameplayInputActionEvent ActionEvent)
{
	if (ShouldBuffInputActionEvent())
	{
		if (GameplayInputBufferSchema->HasInputActionEvent(InputActionTag, ActionEvent))
		{
			const FGameplayInputActionEventConfig InputActionConfig = GameplayInputBufferSchema->
				GetInputActionEventConfig(InputActionTag);

			UGameplayInputActionEventInstance* NewInputActionEvent = NewObject<UGameplayInputActionEventInstance>(this);
			NewInputActionEvent->Initialize(InputActionTag, ActionEvent, InputActionConfig);

			GameplayInputActionEventQueue.Add(NewInputActionEvent);
			return true;
		}
	}
	return false;
}

EGameplayInputBufferScope UGameplayInputBuffer::GetInputBufferScope() const
{
	return BufferScope;
}

bool UGameplayInputBuffer::ShouldBuffInputSourceCommand() const
{
	return BufferScope == EGameplayInputBufferScope::InputSourceOnly ||
		BufferScope == EGameplayInputBufferScope::Both;
}

bool UGameplayInputBuffer::ShouldBuffInputActionEvent() const
{
	return BufferScope == EGameplayInputBufferScope::InputActionOnly ||
		BufferScope == EGameplayInputBufferScope::Both;
}
