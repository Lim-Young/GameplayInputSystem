// Copyright Lim Young.


#include "GameplayInputCommand.h"

FGameplayInputCommand::FGameplayInputCommand()
{
	InputTag = FGameplayTag::EmptyTag;
	InputType = EGameplayInputType::Pressed;
}

FGameplayInputCommand::FGameplayInputCommand(const FGameplayTag InputTag, const EGameplayInputType InputType)
{
	this->InputTag = InputTag;
	this->InputType = InputType;
}

bool FGameplayInputCommand::operator==(const FGameplayInputCommand& Other) const
{
	return InputTag == Other.InputTag && InputType == Other.InputType;
}

void UGameplayInputCommandInstance::Initialize(const FGameplayTag& InInputTag, const EGameplayInputType InInputType,
                                       uint8 InPriority, float InLifetime, float InTimestamp)
{
	InputTag = InInputTag;
	InputType = InInputType;
	Priority = InPriority;
	Lifetime = InLifetime;

	Timestamp = InTimestamp;
}

void UGameplayInputCommandInstance::Initialize(const FGameplayTag& InInputTag, const EGameplayInputType InInputType,
                                       const FGameplayInputCommandConfig& InConfig)
{
	Initialize(InInputTag, InInputType, InConfig.Priority, InConfig.Lifetime, GetWorld()->GetTimeSeconds());
}
