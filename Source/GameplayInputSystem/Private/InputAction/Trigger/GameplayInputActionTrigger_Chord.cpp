// Copyright Lim Young.


#include "InputAction/Trigger/GameplayInputActionTrigger_Chord.h"

void UGameplayInputActionTrigger_Chord::OnTriggerBegin_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	if (ComboTimeout > 0.0f)
	{
		GetTimerManager().SetTimer(ComboTimeoutHandle, this,
		                           &UGameplayInputActionTrigger_Chord::OnComboTimeout,
		                           ComboTimeout, false);
	}
}

void UGameplayInputActionTrigger_Chord::OnInputCommandCaptured_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	if (ComboCommands.Num() == 0)
	{
		return;
	}

	if (CapturedInputCommands.Num() != ComboCommands.Num())
	{
		return;
	}

	if (bInOrder)
	{
		for (int i = 0; i < CapturedInputCommands.Num(); i++)
		{
			if (CapturedInputCommands[i] != ComboCommands[i])
			{
				return;
			}
		}
		FinishTrigger(true);
		return;
	}

	TSet<FGameplayInputSourceCommand> CapturedSet;
	for (const FGameplayInputSourceCommand& Command : CapturedInputCommands)
	{
		if (ComboCommands.Contains(Command))
		{
			CapturedSet.Add(Command);
		}
	}

	if (CapturedSet.Num() == ComboCommands.Num())
	{
		FinishTrigger(true);
	}
}

bool UGameplayInputActionTrigger_Chord::CheckInputCommandCanBeCaptured_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	if (ComboCommands.Num() == 0)
	{
		return false;
	}

	if (CapturedInputCommands.Num() == 0 && bInOrder)
	{
		if (InInputCommand == ComboCommands[0])
		{
			return true;
		}
		return false;
	}

	if (ComboCommands.Contains(InInputCommand))
	{
		return true;
	}

	return false;
}

void UGameplayInputActionTrigger_Chord::OnResetTrigger_Implementation()
{
	if (GetTimerManager().IsTimerActive(ComboTimeoutHandle))
	{
		GetTimerManager().ClearTimer(ComboTimeoutHandle);
	}
}

void UGameplayInputActionTrigger_Chord::OnComboTimeout()
{
	FinishTrigger(false);
}
