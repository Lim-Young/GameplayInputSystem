// Copyright Lim Young.


#include "InputAction/Trigger/GameplayInputActionTrigger_Pressed.h"

bool UGameplayInputActionTrigger_Pressed::CheckInputCommandCanBeCaptured_Implementation(
	const FGameplayInputCommand& InInputCommand)
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

bool UGameplayInputActionTrigger_Pressed::ValidateTriggerCanFinish_Implementation()
{
	if (CapturedInputCommands.Num() == 0)
	{
		return false;
	}

	return true;
}
