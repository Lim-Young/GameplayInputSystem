// Copyright Lim Young.


#include "InputAction/Trigger/GameplayInputActionTrigger_Gated.h"

bool UGameplayInputActionTrigger_Gated::CheckInputCommandCanBeCaptured_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	if (CapturedInputCommands.Num() == 0)
	{
		if (InInputCommand == GateOpenCommand)
		{
			return true;
		}

		return false;
	}

	if (InInputCommand == GateCloseCommand || InInputCommand == TriggerCommand)
	{
		return true;
	}

	return false;
}

void UGameplayInputActionTrigger_Gated::OnInputCommandCaptured_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	if (CapturedInputCommands.Num() > 1)
	{
		if (InInputCommand == GateCloseCommand)
		{
			if (bCompleteOnGateClose)
			{
				FinishTrigger(true);
			}
			else
			{
				FinishTrigger(false, true);
			}
		}
		else if (InInputCommand == TriggerCommand)
		{
			OwningInputAction->BroadcastActionStateEvent(EGameplayInputActionState::Triggered);
		}
	}
}
