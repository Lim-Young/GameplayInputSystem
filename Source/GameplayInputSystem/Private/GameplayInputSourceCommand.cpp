// Copyright Lim Young.


#include "GameplayInputSourceCommand.h"

FGameplayInputSourceCommand::FGameplayInputSourceCommand()
{
	InputSourceTag = FGameplayTag::EmptyTag;
	InputType = EGameplayInputType::Pressed;
}

FGameplayInputSourceCommand::FGameplayInputSourceCommand(const FGameplayTag InputSourceTag, const EGameplayInputType InputType)
{
	this->InputSourceTag = InputSourceTag;
	this->InputType = InputType;
}

void UGameplayInputSourceCommandInstance::Initialize(const FGameplayTag& InInputSourceTag, const EGameplayInputType InInputType,
                                               uint8 InPriority, float InLifetime, float InTimestamp)
{
	InputSourceTag = InInputSourceTag;
	InputType = InInputType;
	Priority = InPriority;
	Lifetime = InLifetime;

	Timestamp = InTimestamp;
}

void UGameplayInputSourceCommandInstance::Initialize(const FGameplayTag& InInputSourceTag, const EGameplayInputType InInputType,
                                               const FGameplayInputSourceCommandConfig& InConfig)
{
	Initialize(InInputSourceTag, InInputType, InConfig.Priority, InConfig.Lifetime, GetWorld()->GetTimeSeconds());
}
