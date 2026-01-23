// Copyright Lim Young.


#include "InputAction/Trigger/GameplayInputActionTrigger_Pressed.h"

bool UGameplayInputActionTrigger_Pressed::CheckInputCommandCanBeCaptured_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	if (InInputCommand.InputSourceTag != InputSourceTag)
	{
		return false;
	}

	if (InInputCommand.InputType == EGameplayInputType::Pressed)
	{
		return true;
	}

	return false;
}

void UGameplayInputActionTrigger_Pressed::OnInputCommandCaptured_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	OwningInputAction->BroadcastActionEvent(EGameplayInputActionEvent::Started);
	OwningInputAction->BroadcastActionEvent(EGameplayInputActionEvent::Completed);
	FinishTrigger(true);
}
