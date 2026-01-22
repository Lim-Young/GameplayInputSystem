// Fill out your copyright notice in the Description page of Project Settings.


#include "InputAction/Trigger/GameplayInputActionTrigger_MultiGated.h"

bool UGameplayInputActionTrigger_MultiGated::CheckCanBeginTrigger_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	return bIsGateOpen;
}

void UGameplayInputActionTrigger_MultiGated::OnTriggerBegin_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
}

bool UGameplayInputActionTrigger_MultiGated::CheckInputCommandCanBeCaptured_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	if (GatesTags.Num() == 0)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT(
			       "UGameplayInputActionTrigger_MultiGated::CheckInputCommandCanBeCaptured_Implementation - GatesTags is empty"
		       ));
		return false;
	}

	if (bIsGateOpen)
	{
		if (InInputCommand == TriggerCommand)
		{
			OwningInputAction->BroadcastActionEvent(EGameplayInputActionEvent::Triggered);
			return false;
		}

		if (GatesTags.Contains(InInputCommand.InputSourceTag))
		{
			if (InInputCommand.InputType == EGameplayInputType::Pressed)
			{
				return false;
			}

			if (CapturedInputCommands.Contains(
				FGameplayInputSourceCommand(InInputCommand.InputSourceTag, EGameplayInputType::Pressed)))
			{
				CapturedInputCommands.Remove(
					FGameplayInputSourceCommand(InInputCommand.InputSourceTag, EGameplayInputType::Pressed));

				// Close the gate if any gate is released
				// TODO: Check Logic
				bIsGateOpen = false;
				bShouldFinishTrigger = true;

				return true;
			}
		}

		return false;
	}

	// Check Gates
	if (!bAllGatesRequired)
	{
		if (GatesTags.Contains(InInputCommand.InputSourceTag))
		{
			if (InInputCommand.InputType == EGameplayInputType::Pressed)
			{
				bIsGateOpen = true;
				return true;
			}
			return false;
		}
	}
	else
	{
		if (bInOrder)
		{
			if (GatesTags[0] == InInputCommand.InputSourceTag &&
				InInputCommand.InputType == EGameplayInputType::Pressed)
			{
				return true;
			}

			return false;
		}

		const int32 CurrentIndex = CapturedInputCommands.Num();
		if (CurrentIndex < GatesTags.Num() &&
			GatesTags[CurrentIndex] == InInputCommand.InputSourceTag &&
			InInputCommand.InputType == EGameplayInputType::Pressed)
		{
			if (CurrentIndex + 1 == GatesTags.Num())
			{
				bIsGateOpen = true;
			}

			return true;
		}
		return false;
	}
	return false;
}

void UGameplayInputActionTrigger_MultiGated::OnInputCommandCaptured_Implementation(
	const FGameplayInputSourceCommand& InInputCommand)
{
	if (bShouldFinishTrigger)
	{
		FinishTrigger(false);
	}
}

void UGameplayInputActionTrigger_MultiGated::OnResetTrigger_Implementation()
{
	bIsGateOpen = false;
	bShouldFinishTrigger = false;
}
