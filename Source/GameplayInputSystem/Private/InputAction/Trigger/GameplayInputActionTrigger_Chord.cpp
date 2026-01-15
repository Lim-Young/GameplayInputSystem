// Copyright Lim Young.


#include "InputAction/Trigger/GameplayInputActionTrigger_Chord.h"

void UGameplayInputActionTrigger_Chord::OnTriggerBegin_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	if (ComboTimeout > 0.0f)
	{
		OwningInputAction->SetActionState(EGameplayInputActionState::Ongoing, false);

		GetTimerManager().SetTimer(ComboTimeoutHandle, this,
								   &UGameplayInputActionTrigger_Chord::OnComboTimeout,
								   ComboTimeout, false);
	}
}

void UGameplayInputActionTrigger_Chord::PreTriggerFinished_Implementation(bool bWasSuccessful, bool bCanceled)
{
	if (GetTimerManager().IsTimerActive(ComboTimeoutHandle))
	{
		GetTimerManager().ClearTimer(ComboTimeoutHandle);
	}
}

bool UGameplayInputActionTrigger_Chord::ValidateTriggerCanFinish_Implementation()
{
	if (ComboCommands.Num() == 0)
	{
		return false;
	}

	if (CapturedInputCommands.Num() != ComboCommands.Num())
	{
		return false;
	}

	if (bInOrder)
	{
		for (int i = 0; i < CapturedInputCommands.Num(); i++)
		{
			if (CapturedInputCommands[i] != ComboCommands[i])
			{
				return false;
			}
		}
		return true;
	}

	TSet<FGameplayInputSourceCommand> CapturedSet;
	for (const FGameplayInputSourceCommand& Command : CapturedInputCommands)
	{
		if (ComboCommands.Contains(Command))
		{
			CapturedSet.Add(Command);
		}
	}

	return CapturedSet.Num() == ComboCommands.Num();
}

bool UGameplayInputActionTrigger_Chord::CheckInputCommandCanBeCaptured_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	if (ComboCommands.Num() == 0)
	{
		return false;
	}

	if (ComboCommands.Contains(InInputCommand))
	{
		return true;
	}

	return false;
}

void UGameplayInputActionTrigger_Chord::OnComboTimeout()
{
	FinishTrigger(false);
}
