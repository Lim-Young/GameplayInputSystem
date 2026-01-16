// Copyright Lim Young.


#include "InputAction/GameplayInputActionSet.h"

#include "GameplayInputSubsystem.h"

bool UGameplayInputActionTrigger::CheckInputCommandCanBeCaptured_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	return false;
}

bool UGameplayInputActionTrigger::CaptureInputCommand(const FGameplayInputSourceCommand& InInputCommand)
{
	// TODO:
	if (CheckInputCommandCanBeCaptured(InInputCommand))
	{
		CapturedInputCommands.Add(InInputCommand);

		if (OwningInputAction->CurrentActionState != EGameplayInputActionState::Started)
		{
			if (CheckCanBeginTrigger(InInputCommand))
			{
				BeginTrigger(InInputCommand);
			}
		}

		// if (ValidateTriggerCanFinish())
		// {
		// 	FinishTrigger(true);
		// }
		OnInputCommandCaptured(InInputCommand);

		return true;
	};

	CapturedInputCommands.Empty();
	return false;
}

bool UGameplayInputActionTrigger::CheckCanBeginTrigger_Implementation(const FGameplayInputSourceCommand& InInputCommand)
{
	return true;
}

void UGameplayInputActionTrigger::OnInputCommandCaptured_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
}

// bool UGameplayInputActionTrigger::ValidateTriggerCanFinish_Implementation()
// {
// 	return false;
// }

void UGameplayInputActionTrigger::BeginTrigger(const FGameplayInputSourceCommand& InInputCommand)
{
	OwningInputAction->SetActionState(EGameplayInputActionState::Started);

	OnTriggerBegin(InInputCommand);
}

void UGameplayInputActionTrigger::ResetTrigger()
{
	bReleaseInputCommands = false;
	CapturedInputCommands.Empty();
	OnResetTrigger();
}

void UGameplayInputActionTrigger::OnResetTrigger_Implementation()
{
}

void UGameplayInputActionTrigger::OnTriggerBegin_Implementation(const FGameplayInputSourceCommand& InInputCommand)
{
}

void UGameplayInputActionTrigger::PreTriggerFinished_Implementation(bool bWasSuccessful, bool bCanceled)
{
}

void UGameplayInputActionTrigger::FinishTrigger(const bool bWasSuccessful, const bool bCanceled)
{
	PreTriggerFinished(bWasSuccessful, bCanceled);
	OwningInputAction->FinishAction(this, bWasSuccessful, bCanceled);
}

FTimerManager& UGameplayInputActionTrigger::GetTimerManager() const
{
	return GetWorld()->GetTimerManager();
}

const TArray<FGameplayInputSourceCommand>& UGameplayInputActionTrigger::GetCapturedInputCommands() const
{
	return CapturedInputCommands;
}

bool UGameplayInputActionTrigger::IsReleasingInputCommands() const
{
	return bReleaseInputCommands;
}

void UGameplayInputActionTrigger::SetReleaseInputCommands(const bool bInReleaseInputCommands)
{
	bReleaseInputCommands = bInReleaseInputCommands;
}

bool UGameplayInputAction::CheckCanActivateAction(const FGameplayInputSourceCommand& InInputCommand)
{
	// If the action is already ongoing, do not activate again
	// if (CurrentActionState == EGameplayInputActionState::Ongoing)
	// {
	// 	return false;
	// }

	for (UGameplayInputActionTrigger* Trigger : Triggers)
	{
		if (Trigger && !Trigger->IsReleasingInputCommands() && Trigger->CheckInputCommandCanBeCaptured(InInputCommand))
		{
			return true;
		}
	}

	return false;
}

void UGameplayInputAction::SetActionState(const EGameplayInputActionState NewActionState, bool bBroadcastEvent)
{
	CurrentActionState = NewActionState;
	if (bBroadcastEvent)
	{
		BroadcastActionStateEvent(NewActionState);
	}
}

EGameplayInputActionState UGameplayInputAction::GetCurrentActionState() const
{
	return CurrentActionState;
}

void UGameplayInputAction::BroadcastActionStateEvent(const EGameplayInputActionState ActionState) const
{
	check(OwningActionSet);
	OwningActionSet->TriggerGameplayInputAction(InputActionTag, ActionState);
}

void UGameplayInputAction::FinishAction(UGameplayInputActionTrigger* ExecutingTrigger,
                                        const bool bWasSuccessful, const bool bCanceled)
{
	if (bWasSuccessful)
	{
		SetActionState(EGameplayInputActionState::Completed);
	}
	else
	{
		if (bCanceled)
		{
			SetActionState(EGameplayInputActionState::Canceled);
		}
		else
		{
			const TArray<FGameplayInputSourceCommand> CapturedInputCommands = ExecutingTrigger->
				GetCapturedInputCommands();
			ExecutingTrigger->SetReleaseInputCommands(true);
			// Failed, We should re-inject the captured input commands back to the action set for re-processing
			for (FGameplayInputSourceCommand InputCommand : CapturedInputCommands)
			{
				// Pass down to prevent higher-priority Action Sets from capturing again, which could cause an infinite loop.
				OwningActionSet->HandleInput(InputCommand, true, Priority);
			}
			ExecutingTrigger->SetReleaseInputCommands(false);
		}
	}

	// Reset All Triggers
	for (UGameplayInputActionTrigger* Trigger : Triggers)
	{
		if (Trigger)
		{
			Trigger->ResetTrigger();
		}
	}

	SetActionState(EGameplayInputActionState::Idle, false);
}

void UGameplayInputAction::BeginAction(const FGameplayInputSourceCommand& InInputCommand)
{
	for (UGameplayInputActionTrigger* Trigger : Triggers)
	{
		if (Trigger)
		{
			Trigger->CaptureInputCommand(InInputCommand);
		}
	}
}

UGameplayInputActionSet* UGameplayInputActionSet::CreateByTemplateObject(const UGameplayInputActionSet* TemplateObject,
                                                                         UObject* Outer,
                                                                         UGameplayInputSubsystem* OwningSubsystem,
                                                                         uint8 InPriority)
{
	if (!TemplateObject)
	{
		return nullptr;
	}

	UGameplayInputActionSet* InputActionSetInstance = DuplicateObject<UGameplayInputActionSet>(
		TemplateObject, Outer);

	// Make sure outer chain is correct
	TArray<TObjectPtr<UGameplayInputActionTrigger>> CopyOfTriggers;
	TArray<TObjectPtr<UGameplayInputAction>> CopyOfInputActions;
	for (const UGameplayInputAction* InputAction : InputActionSetInstance->Actions)
	{
		if (!InputAction)
		{
			continue;
		}

		UGameplayInputAction* CopyOfInputAction = DuplicateObject<UGameplayInputAction>(
			InputAction, InputActionSetInstance);
		CopyOfInputAction->OwningActionSet = InputActionSetInstance;

		CopyOfTriggers.Empty();
		for (const UGameplayInputActionTrigger* Trigger : InputAction->Triggers)
		{
			if (!Trigger)
			{
				continue;
			}

			UGameplayInputActionTrigger* CopyOfTrigger = DuplicateObject<UGameplayInputActionTrigger>(
				Trigger, CopyOfInputAction);
			CopyOfTrigger->OwningInputAction = CopyOfInputAction;
			CopyOfTriggers.Add(CopyOfTrigger);
		}
		CopyOfInputAction->Triggers = CopyOfTriggers;

		CopyOfInputActions.Add(CopyOfInputAction);
	}
	InputActionSetInstance->Actions = CopyOfInputActions;

	InputActionSetInstance->Priority = InPriority;
	InputActionSetInstance->OwningSubsystem = OwningSubsystem;
	return InputActionSetInstance;
}

bool UGameplayInputActionSet::HandleInput(const FGameplayInputSourceCommand& InInputCommand,
                                          const bool bClampedByHigherPriority,
                                          const uint8 CustomPriority)
{
	TArray<UGameplayInputAction*> MaxPriorityActions;
	uint8 MaxPriority = 0;
	for (UGameplayInputAction* InputAction : Actions)
	{
		if (InputAction && InputAction->CheckCanActivateAction(InInputCommand))
		{
			if (bClampedByHigherPriority && InputAction->Priority >= CustomPriority)
			{
				continue;
			}

			if (InputAction->Priority > MaxPriority)
			{
				MaxPriority = InputAction->Priority;
				MaxPriorityActions.Empty();
				MaxPriorityActions.Add(InputAction);
			}
			else if (InputAction->Priority == MaxPriority)
			{
				MaxPriorityActions.Add(InputAction);
			}
		}
	}

	if (MaxPriorityActions.Num() == 0)
	{
		return false;
	}

	for (UGameplayInputAction* InputAction : MaxPriorityActions)
	{
		InputAction->BeginAction(InInputCommand);
	}
	return true;
}

void UGameplayInputActionSet::TriggerGameplayInputAction(const FGameplayTag& InputActionTag,
                                                         const EGameplayInputActionState ActionState) const
{
	if (IsValid(OwningSubsystem))
	{
		OwningSubsystem->ForceTriggerGameplayInputAction(InputActionTag, ActionState);
	}
}

#if WITH_EDITOR
void UGameplayInputActionSet::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	//  Auto Create Input Action Instance When Actions Array Add New Element
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() ==
		GET_MEMBER_NAME_CHECKED(UGameplayInputActionSet, Actions))
	{
		const int32 Index = PropertyChangedEvent.GetArrayIndex(
			GET_MEMBER_NAME_CHECKED(UGameplayInputActionSet, Actions).ToString());
		if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd ||
			PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate)
		{
			for (int i = 0; i < Actions.Num(); i++)
			{
				if (i != Index && Actions[i] && Actions[i]->InputActionTag == FGameplayTag::EmptyTag)
				{
					// No new Actions are allowed.
					FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(
						                     "GameplayInputActionSet: New Action cannot have an empty InputActionTag."));
					Actions.RemoveAt(Index);
					return;
				}
			}

			if (Index >= 0 && Actions.IsValidIndex(Index))
			{
				TObjectPtr<UGameplayInputAction>& Action = Actions[Index];
				if (!Action)
				{
					Action = NewObject<UGameplayInputAction>(this);
				}
			}
		}

		if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ValueSet ||
			PropertyChangedEvent.ChangeType == EPropertyChangeType::ResetToDefault ||
			PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate)
		{
			if (const TObjectPtr<UGameplayInputAction> CurrentAction = Actions[Index])
			{
				for (int i = 0; i < Actions.Num(); i++)
				{
					if (i != Index && Actions[i] && Actions[i]->InputActionTag == CurrentAction->InputActionTag)
					{
						// Duplicate input action tags are not allowed.
						FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(
							                     "GameplayInputActionSet: Duplicate InputActionTag are not allowed."));
						CurrentAction->InputActionTag = FGameplayTag::EmptyTag;
						break;
					}
				}
			}
		}
	}
}
#endif
