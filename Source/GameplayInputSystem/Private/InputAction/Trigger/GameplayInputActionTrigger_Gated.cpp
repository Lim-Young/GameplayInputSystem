// Copyright Lim Young.


#include "InputAction/Trigger/GameplayInputActionTrigger_Gated.h"

bool UGameplayInputActionTrigger_Gated::CheckCanActivate_Implementation(FGameplayInputSourceCommand InInputCommand)
{
	return Super::CheckCanActivate_Implementation(InInputCommand);
}

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
			bIsGateOpen = false;
			if (!OwningInputAction->IsInactive())
			{
				// TODO: 等待进一步重构，明确Check函数可修改状态，并确保Action处理时从低优先级往高优先级处理，这样高优先级Action可捕获到低优先级Action的状态变化
				// 目前实现上是可行的，但不规范
				OwningInputAction->SetActionState(EGameplayInputActionState::Inactive);
				return true;
			}

			return false;
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
	else
	{
		if (!bIsGateOpen)
		{
			OwningInputAction->BroadcastActionEvent(EGameplayInputActionEvent::Canceled);
			FinishTrigger(false, true);
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
