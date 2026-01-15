// Copyright Lim Young.


#include "GameplayInputSubsystem.h"

#include "GameplayInputSystemTags.h"
#include "InputAction/GameplayInputActionSet.h"

void UGameplayInputSubsystem::InjectGameplayInput(const FGameplayTag& InputSourceTag, const EGameplayInputType InputType)
{
	if (!InputSourceTag.MatchesTag(GameplayInput))
	{
		return;
	}

	if (GameplayInputArbiters.Num() > 0)
	{
		bool bHasValidArbiter = false;

		for (auto& ArbiterPair : GameplayInputArbiters)
		{
			if (!ArbiterPair.Value)
			{
				continue;
			}

			bHasValidArbiter |= ArbiterPair.Value->ReceiveGameplayInput(InputSourceTag, InputType);
		}

		if (bHasValidArbiter)
		{
			return;
		}
	}

	ActiveGameplayInputActionSets.HeapTop()->HandleInput(FGameplayInputCommand(InputSourceTag, InputType));

	BroadcastGameplayInputEvent(InputSourceTag, InputType);
}

void UGameplayInputSubsystem::CreateAndRegisterGameplayInputArbiter(UGameplayInputDocket* InGameplayInputDocker)
{
	if (GameplayInputArbiters.Contains(InGameplayInputDocker))
	{
		// Cancel and remove existing arbiter
		GameplayInputArbiters[InGameplayInputDocker]->Cancel();
	}

	UGameplayInputArbiter* NewArbiter = NewObject<UGameplayInputArbiter>(this);
	NewArbiter->Initialize(InGameplayInputDocker);
	GameplayInputArbiters.Add(InGameplayInputDocker, NewArbiter);

	NewArbiter->Start();
}

void UGameplayInputSubsystem::FinishAndUnregisterGameplayInputArbiter(UGameplayInputDocket* InGameplayInputDocker)
{
	if (GameplayInputArbiters.Contains(InGameplayInputDocker))
	{
		UGameplayInputCommandInstance* ResultCommandInstance;
		if (GameplayInputArbiters[InGameplayInputDocker]->Finish(ResultCommandInstance))
		{
			BroadcastGameplayInputEvent(ResultCommandInstance->InputSourceTag, ResultCommandInstance->InputType);
		}

		GameplayInputArbiters.Remove(InGameplayInputDocker);
	}
}

void UGameplayInputSubsystem::AddGameplayInputActionSet(UGameplayInputActionSet* InInputActionSet, uint8 Priority)
{
	if (!InInputActionSet)
	{
		return;
	}

	if (GameplayInputActionSetMapping.Contains(InInputActionSet))
	{
		// GameplayInputActionSetMapping[InInputActionSet]->Priority = Priority;
		RemoveGameplayInputActionSet(InInputActionSet);
		return;
	}

	UGameplayInputActionSet* InputActionSetInstance = UGameplayInputActionSet::CreateByTemplateObject(
		InInputActionSet, this, this, Priority);

	ActiveGameplayInputActionSets.HeapPush(InputActionSetInstance,
	                                       [](const UGameplayInputActionSet& A,
	                                          const UGameplayInputActionSet& B)
	                                       {
		                                       return A.Priority > B.Priority;
	                                       });
	GameplayInputActionSetMapping.Add(InInputActionSet, InputActionSetInstance);
}

void UGameplayInputSubsystem::RemoveGameplayInputActionSet(UGameplayInputActionSet* InInputActionSet)
{
	if (!InInputActionSet)
	{
		return;
	}

	if (GameplayInputActionSetMapping.Contains(InInputActionSet))
	{
		UGameplayInputActionSet* MappedActionSet = GameplayInputActionSetMapping[InInputActionSet];
		ActiveGameplayInputActionSets.Remove(MappedActionSet);
		GameplayInputActionSetMapping.Remove(InInputActionSet);
	}
}

void UGameplayInputSubsystem::ForceTriggerGameplayInputAction(const FGameplayTag& InputActionTag,
                                                              const EGameplayInputActionState ActionState)
{
	BroadcastGameplayInputActionTriggered(InputActionTag, ActionState);
}

void UGameplayInputSubsystem::BroadcastGameplayInputEvent(const FGameplayTag& InputSourceTag,
                                                          const EGameplayInputType InputType) const
{
	OnGameplayInputEvent.Broadcast(InputSourceTag, InputType);
}

void UGameplayInputSubsystem::BroadcastGameplayInputActionTriggered(const FGameplayTag& InputActionTag,
                                                                    const EGameplayInputActionState ActionState) const
{
	OnGameplayInputActionTriggered.Broadcast(InputActionTag, ActionState);
}
