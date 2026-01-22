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
	if (GameplayInputBuffers.Num() > 0)
	{
		for (auto& ArbiterPair : GameplayInputBuffers)
		{
			if (!ArbiterPair.Value)
			{
				continue;
			}

			if (ArbiterPair.Value->ShouldBuffInputSourceCommand())
			{
				bIsCapturedByArbiter |= ArbiterPair.Value->ReceiveGameplayInputSourceCommand(InputSourceTag, InputType);
			}
		}
	}

	if (!bIsCapturedByArbiter)
	{
		BroadcastGameplayInputEvent(InputSourceTag, InputType);
	}

	if (ActiveGameplayInputActionSets.IsEmpty())
	{
		return;
	}

	ActiveGameplayInputActionSets.HeapTop()->HandleInput(FGameplayInputSourceCommand(InputSourceTag, InputType));
}

void UGameplayInputSubsystem::CreateAndRegisterGameplayInputBuffer(UGameplayInputBufferSchema* InGameplayInputDocker,
                                                                   const EGameplayInputBufferScope InputBufferScope)
{
	if (GameplayInputBuffers.Contains(InGameplayInputDocker))
	{
		// Cancel and remove existing arbiter
		GameplayInputBuffers[InGameplayInputDocker]->Cancel();
	}

	UGameplayInputBuffer* NewBuffer = NewObject<UGameplayInputBuffer>(this);
	NewBuffer->Initialize(InGameplayInputDocker, InputBufferScope);
	GameplayInputBuffers.Add(InGameplayInputDocker, NewBuffer);

	NewBuffer->Start();
}

void UGameplayInputSubsystem::FinishAndUnregisterGameplayInputBuffer(UGameplayInputBufferSchema* InGameplayInputDocker)
{
	if (GameplayInputBuffers.Contains(InGameplayInputDocker))
	{
		UGameplayInputSourceCommandInstance* ResultInputSourceCommandInstance;
		UGameplayInputActionEventInstance* ResultActionEventInstance;
		UGameplayInputBuffer* Buffer = GameplayInputBuffers[InGameplayInputDocker];

		if (Buffer->Finish(ResultInputSourceCommandInstance, ResultActionEventInstance))
		{
			if (Buffer->ShouldBuffInputSourceCommand())
			{
				BroadcastGameplayInputEvent(ResultInputSourceCommandInstance->InputSourceTag,
				                            ResultInputSourceCommandInstance->InputType);
			}

			if (Buffer->ShouldBuffInputActionEvent())
			{
				BroadcastGameplayInputActionTriggered(ResultActionEventInstance->InputActionTag,
				                                      ResultActionEventInstance->InputActionEvent);
			}
		}

		GameplayInputBuffers.Remove(InGameplayInputDocker);
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
                                                              const EGameplayInputActionEvent ActionState)
{
	if (GameplayInputBuffers.Num() > 0)
	{
		bool bIsCapturedByArbiter = false;
		for (auto& ArbiterPair : GameplayInputBuffers)
		{
			if (!ArbiterPair.Value)
			{
				continue;
			}

			if (ArbiterPair.Value->ShouldBuffInputActionEvent())
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
	OnGameplayInputSourceEvent.Broadcast(InputSourceTag, InputType);
}

void UGameplayInputSubsystem::BroadcastGameplayInputActionTriggered(const FGameplayTag& InputActionTag,
                                                                    const EGameplayInputActionEvent ActionState) const
{
	OnGameplayInputActionTriggered.Broadcast(InputActionTag, ActionState);
}
