// Copyright Lim Young.


#include "InputAction/Trigger/GameplayInputActionTrigger_Gated.h"

void UGameplayInputActionTrigger_Gated::OnTriggerBegin_Implementation(const FGameplayInputSourceCommand& InInputCommand)
{
	OwningInputAction->SetActionState(EGameplayInputActionState::Pending);
}

bool UGameplayInputActionTrigger_Gated::CheckInputCommandCanBeCaptured_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	if (bActionGatedMode)
	{
		const UGameplayInputAction* InputAction;
		if (OwningInputAction->GetOwningActionSet()->FindActionByTag(GatedActionTag, InputAction)
			&& InputAction->IsActive() && InInputCommand.InputSourceTag == TriggerInputSourceTag)
		{
			bIsGateOpen = true;
			return true;
		}

		if (bIsGateOpen)
		{
			FinishTrigger(false, true);
		}
		bIsGateOpen = false;
		return false;
	}

	if (CapturedInputCommands.Num() == 0)
	{
		if (InInputCommand == GateOpenCommand)
		{
			return true;
		}

		return false;
	}

	if (InInputCommand == GateCloseCommand || InInputCommand.InputSourceTag == TriggerInputSourceTag)
	{
		return true;
	}

	return false;
}

void UGameplayInputActionTrigger_Gated::OnInputCommandCaptured_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	if (!bActionGatedMode)
	{
		if (InInputCommand == GateCloseCommand)
		{
			FinishTrigger(false, true);
			return;
		}
	}

	if (InInputCommand.InputSourceTag == TriggerInputSourceTag)
	{
		if (InInputCommand.InputType == EGameplayInputType::Pressed)
		{
			OwningInputAction->SetActionState(EGameplayInputActionState::Active);
			OwningInputAction->BroadcastActionEvent(EGameplayInputActionEvent::Started);
		}
		else
		{
			OwningInputAction->SetActionState(EGameplayInputActionState::Pending);
			OwningInputAction->BroadcastActionEvent(EGameplayInputActionEvent::Completed);
		}
	}
}
