// Copyright Lim Young.


#include "GameplayInputArbiter.h"

void UGameplayInputCommand::Initialize(const FGameplayTag& InInputTag, const EGameplayInputType InInputType,
                                       uint8 InPriority, float InLifetime, float InTimestamp)
{
	InputTag = InInputTag;
	InputType = InInputType;
	Priority = InPriority;
	Lifetime = InLifetime;

	Timestamp = InTimestamp;
}

void UGameplayInputCommand::Initialize(const FGameplayTag& InInputTag, const EGameplayInputType InInputType,
                                       const FGameplayInputCommandConfig& InConfig)
{
	Initialize(InInputTag, InInputType, InConfig.Priority, InConfig.Lifetime, GetWorld()->GetTimeSeconds());
}

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