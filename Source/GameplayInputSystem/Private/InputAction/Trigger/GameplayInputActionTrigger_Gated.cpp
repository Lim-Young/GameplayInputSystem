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
			&& InputAction->IsActive())
		{
			bIsGateOpen = true;
		}
		else
		{
			// TODO: 目前Gated触发器的Action在被关闭时不会自动关闭触发器，状态会一直保持Pending，目前不会有太大影响，后续需要处理好状态问题
			bIsGateOpen = false;
		}

		if (bIsGateOpen && InInputCommand.InputSourceTag == TriggerInputSourceTag)
		{
			return true;
		}
		// if (bIsGateOpen)
		// {
		// 	FinishTrigger(false, true);
		// }
		// bIsGateOpen = false;
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
