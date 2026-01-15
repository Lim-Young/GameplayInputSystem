// Copyright Lim Young.


#include "InputAction/Trigger/GameplayInputActionTrigger_Released.h"

bool UGameplayInputActionTrigger_Released::CheckInputCommandCanBeCaptured_Implementation(
	const FGameplayInputCommand& InInputCommand)
{
	if (InInputCommand.InputSourceTag != InputSourceTag)
	{
		return false;
	}

	if (InInputCommand.InputType == EGameplayInputType::Released)
	{
		return true;
	}

	return false;
}

bool UGameplayInputActionTrigger_Released::ValidateTriggerCanFinish_Implementation()
{
	if (CapturedInputCommands.Num() == 0)
	{
		return false;
	}

	return true;
}
