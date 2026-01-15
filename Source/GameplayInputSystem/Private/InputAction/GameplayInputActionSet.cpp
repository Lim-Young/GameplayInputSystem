// Copyright Lim Young.


#include "InputAction/GameplayInputActionSet.h"

#include "GameplayInputSubsystem.h"

bool UGameplayInputActionTrigger::CheckInputCommandCanBeCaptured_Implementation(
	const FGameplayInputCommand& InInputCommand)
{
	return false;
}

bool UGameplayInputActionTrigger::CaptureInputCommand(const FGameplayInputCommand& InInputCommand)
{
	// TODO:
	if (CheckInputCommandCanBeCaptured(InInputCommand))
	{
		CapturedInputCommands.Add(InInputCommand);
		return true;
	};

	CapturedInputCommands.Empty();
	return false;
}

bool UGameplayInputActionTrigger::ValidateTriggerCanFinish_Implementation()
{
	return false;
}

void UGameplayInputActionTrigger::BeginTrigger(const FGameplayInputCommand& InInputCommand)
{
	OwningInputAction->SetActionState(EGameplayInputActionState::Started);

	OnTriggerBegin(InInputCommand);
	if (ValidateTriggerCanFinish())
	{
		FinishTrigger(true);
	}
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

void UGameplayInputActionTrigger::OnTriggerBegin_Implementation(const FGameplayInputCommand& InInputCommand)
{
	FinishTrigger(true);
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

const TArray<FGameplayInputCommand>& UGameplayInputActionTrigger::GetCapturedInputCommands() const
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

bool UGameplayInputAction::CheckCanActivateAction(const FGameplayInputCommand& InInputCommand)
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
		check(OwningActionSet);
		OwningActionSet->TriggerGameplayInputAction(ActionTag, NewActionState);
	}
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
			const TArray<FGameplayInputCommand> CapturedInputCommands = ExecutingTrigger->GetCapturedInputCommands();
			ExecutingTrigger->SetReleaseInputCommands(true);
			// Failed, We should re-inject the captured input commands back to the action set for re-processing
			for (FGameplayInputCommand InputCommand : CapturedInputCommands)
			{
				OwningActionSet->HandleInput(InputCommand);
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

void UGameplayInputAction::BeginAction(const FGameplayInputCommand& InInputCommand)
{
	for (UGameplayInputActionTrigger* Trigger : Triggers)
	{
		if (Trigger && Trigger->CaptureInputCommand(InInputCommand))
		{
			Trigger->BeginTrigger(InInputCommand);
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

bool UGameplayInputActionSet::HandleInput(const FGameplayInputCommand& InInputCommand)
{
	TArray<UGameplayInputAction*> MaxPriorityActions;
	uint8 MaxPriority = 0;
	for (UGameplayInputAction* InputAction : Actions)
	{
		if (InputAction && InputAction->CheckCanActivateAction(InInputCommand))
		{
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

void UGameplayInputActionSet::TriggerGameplayInputAction(const FGameplayTag& ActionTag,
                                                         const EGameplayInputActionState ActionState) const
{
	if (IsValid(OwningSubsystem))
	{
		OwningSubsystem->ForceTriggerGameplayInputAction(ActionTag, ActionState);
	}
}
