// Copyright Lim Young.


#include "InputAction/Trigger/GameplayInputActionTrigger_Pressed.h"

bool UGameplayInputActionTrigger_Pressed::CheckInputCommandCanBeCaptured_Implementation(
	const FGameplayInputCommand& InInputCommand)
{
	if (PressedCommand == InInputCommand)
	{
		return true;
	}
	return false;
}

bool UGameplayInputActionTrigger_Pressed::ValidateTriggerCanFinish_Implementation()
{
	if (CapturedInputCommands.Num() == 0)
	{
		return false;
	}

	if (CapturedInputCommands[0] == PressedCommand)
	{
		return true;
	}
	return false;
}