// Copyright Lim Young.


#include "InputAction/Trigger/GameplayInputActionTrigger_Hold.h"

bool UGameplayInputActionTrigger_Hold::CheckInputCommandCanBeCaptured_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	if (InInputCommand.InputSourceTag != InputSourceTag)
	{
		return false;
	}
	
	if (bIsHolding && InInputCommand.InputType == EGameplayInputType::Released)
	{
		OwningInputAction->SetActionState(EGameplayInputActionState::Inactive);
	}

	return true;
}

void UGameplayInputActionTrigger_Hold::OnInputCommandCaptured_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	if (bIsHolding)
	{
		if (InInputCommand.InputType == EGameplayInputType::Released)
		{
			bIsHolding = false;
			OwningInputAction->BroadcastActionEvent(EGameplayInputActionEvent::Completed);
			FinishTrigger(true);
		}

		return;
	}

	if (InInputCommand.InputType == EGameplayInputType::Pressed)
	{
		bIsHolding = true;
		OwningInputAction->BroadcastActionEvent(EGameplayInputActionEvent::Started);
	}
}
