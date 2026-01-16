// Copyright Lim Young.


#include "GameplayInputSubsystem.h"

#include "GameplayInputSystemTags.h"
#include "InputAction/GameplayInputActionSet.h"

void UGameplayInputSubsystem::InjectGameplayInput(const FGameplayTag& InputSourceTag,
                                                  const EGameplayInputType InputType)
{
	if (!InputSourceTag.MatchesTag(GameplayInput))
	{
		return;
	}

	bool bIsCapturedByArbiter = false;
	if (GameplayInputArbiters.Num() > 0)
	{
		for (auto& ArbiterPair : GameplayInputArbiters)
		{
			if (!ArbiterPair.Value)
			{
				continue;
			}

			if (ArbiterPair.Value->ShouldMatchInputSourceCommand())
			{
				bIsCapturedByArbiter |= ArbiterPair.Value->ReceiveGameplayInputSourceCommand(InputSourceTag, InputType);
			}
		}
	}

	if (!bIsCapturedByArbiter)
	{
		BroadcastGameplayInputEvent(InputSourceTag, InputType);
	}

	ActiveGameplayInputActionSets.HeapTop()->HandleInput(FGameplayInputSourceCommand(InputSourceTag, InputType));
}

void UGameplayInputSubsystem::CreateAndRegisterGameplayInputArbiter(UGameplayInputDocket* InGameplayInputDocker,
                                                                    const EArbiterCommandMatchMode MatchMode)
{
	if (GameplayInputArbiters.Contains(InGameplayInputDocker))
	{
		// Cancel and remove existing arbiter
		GameplayInputArbiters[InGameplayInputDocker]->Cancel();
	}

	UGameplayInputArbiter* NewArbiter = NewObject<UGameplayInputArbiter>(this);
	NewArbiter->Initialize(InGameplayInputDocker, MatchMode);
	GameplayInputArbiters.Add(InGameplayInputDocker, NewArbiter);

	NewArbiter->Start();
}

void UGameplayInputSubsystem::FinishAndUnregisterGameplayInputArbiter(UGameplayInputDocket* InGameplayInputDocker)
{
	if (GameplayInputArbiters.Contains(InGameplayInputDocker))
	{
		UGameplayInputSourceCommandInstance* ResultInputSourceCommandInstance;
		UGameplayInputActionEventInstance* ResultActionEventInstance;
		UGameplayInputArbiter* Arbiter = GameplayInputArbiters[InGameplayInputDocker];

		if (Arbiter->Finish(ResultInputSourceCommandInstance, ResultActionEventInstance))
		{
			if (Arbiter->ShouldMatchInputSourceCommand())
			{
				BroadcastGameplayInputEvent(ResultInputSourceCommandInstance->InputSourceTag,
				                            ResultInputSourceCommandInstance->InputType);
			}

			if (Arbiter->ShouldMatchInputActionEvent())
			{
				BroadcastGameplayInputActionTriggered(ResultActionEventInstance->InputActionTag,
				                                      ResultActionEventInstance->InputActionState);
			}
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
	if (GameplayInputArbiters.Num() > 0)
	{
		bool bIsCapturedByArbiter = false;
		for (auto& ArbiterPair : GameplayInputArbiters)
		{
			if (!ArbiterPair.Value)
			{
				continue;
			}

			if (ArbiterPair.Value->ShouldMatchInputActionEvent())
			{
				bIsCapturedByArbiter |= ArbiterPair.Value->ReceiveGameplayInputActionEvent(InputActionTag, ActionState);
			}
		}

		if (bIsCapturedByArbiter)
		{
			return;
		}
	}

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
