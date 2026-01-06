// Copyright Lim Young.


#include "GameplayInputDocket.h"

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
