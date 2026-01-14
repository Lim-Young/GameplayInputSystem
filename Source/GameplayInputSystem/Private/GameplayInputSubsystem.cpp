// Copyright Lim Young.


#include "GameplayInputSubsystem.h"

#include "GameplayInputSystemTags.h"

void UGameplayInputSubsystem::InjectGameplayInput(const FGameplayTag& InputTag, const EGameplayInputType InputType)
{
	if (!InputTag.MatchesTag(GameplayInput))
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

			bHasValidArbiter |= ArbiterPair.Value->ReceiveGameplayInput(InputTag, InputType);
		}

		if (bHasValidArbiter)
		{
			return;
		}
	}


	BroadcastGameplayInputEvent(InputTag, InputType);
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
		UGameplayInputCommand* ResultCommand;
		if (GameplayInputArbiters[InGameplayInputDocker]->Finish(ResultCommand))
		{
			BroadcastGameplayInputEvent(ResultCommand->InputTag, ResultCommand->InputType);
		}

		GameplayInputArbiters.Remove(InGameplayInputDocker);
	}
}

void UGameplayInputSubsystem::BroadcastGameplayInputEvent(const FGameplayTag& InputTag,
                                                          const EGameplayInputType InputType) const
{
	OnGameplayInputEvent.Broadcast(InputTag, InputType);
}
