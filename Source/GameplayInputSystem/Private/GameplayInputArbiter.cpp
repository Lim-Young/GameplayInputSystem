// Copyright Lim Young.


#include "GameplayInputArbiter.h"

#include "GameplayInputDocket.h"

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