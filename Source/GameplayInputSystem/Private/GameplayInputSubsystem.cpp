// Copyright Lim Young.


#include "GameplayInputSubsystem.h"

UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayInput, "GameplayInput",
                               "Tag used to identify gameplay input actions. This tag is used to differentiate between gameplay-related inputs and other types of inputs.")

bool UGameplayInputDocket::HasCommandForTag(const FGameplayTag& InputTag) const
{
	return InputCommands.Contains(InputTag);
}

FGameplayInputCommandConfig& UGameplayInputDocket::GetCommandConfigForTag(const FGameplayTag& InputTag)
{
	return InputCommands.FindChecked(InputTag);
}

void UGameplayInputArbiter::Initialize(UGameplayInputDocket* InGameplayInputDocker)
{
	GameplayInputDocker = InGameplayInputDocker;
}

void UGameplayInputArbiter::Start()
{
	GameplayInputCommandQueue.Empty();
}

void UGameplayInputArbiter::Cancel()
{
	GameplayInputCommandQueue.Empty();
}

bool UGameplayInputArbiter::Finish(UGameplayInputCommand*& ResultCommand)
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	TObjectPtr<UGameplayInputCommand> HighestPriorityCommand = nullptr;
	uint8 HighestPriority = 0;

	for (const TObjectPtr<UGameplayInputCommand> CurrentCommand : GameplayInputCommandQueue)
	{
		if (!CurrentCommand)
		{
			continue;
		}

		// Command expired
		if (CurrentCommand->Lifetime != 0.0f && (CurrentTime - CurrentCommand->Timestamp) > CurrentCommand->Lifetime)
		{
			continue;
		}

		// Find Priority Command
		if (CurrentCommand->Priority >= HighestPriority)
		{
			HighestPriority = CurrentCommand->Priority;
			HighestPriorityCommand = CurrentCommand;
		}
	}

	if (HighestPriorityCommand)
	{
		ResultCommand = HighestPriorityCommand;
		return true;
	}

	return false;
}

bool UGameplayInputArbiter::ReceiveGameplayInput(const FGameplayTag InputTag, EGameplayInputType InputType)
{
	if (GameplayInputDocker->HasCommandForTag(InputTag))
	{
		const FGameplayInputCommandConfig InputCommandConfig = GameplayInputDocker->GetCommandConfigForTag(InputTag);

		UGameplayInputCommand* NewInputCommand = NewObject<UGameplayInputCommand>(this);
		NewInputCommand->Initialize(InputTag, InputType, InputCommandConfig);

		GameplayInputCommandQueue.Add(NewInputCommand);
		return true;
	}
	return false;
}

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

	OnGameplayInputEvent.Broadcast(InputTag, InputType);
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
			OnGameplayInputEvent.Broadcast(ResultCommand->InputTag, ResultCommand->InputType);
		}

		GameplayInputArbiters.Remove(InGameplayInputDocker);
	}
}
