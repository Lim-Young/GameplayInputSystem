// Copyright Lim Young.


#include "GameplayInputCommand.h"

FGameplayInputCommandType::FGameplayInputCommandType()
{
	InputTag = FGameplayTag::EmptyTag;
	InputType = EGameplayInputType::Action;
}

FGameplayInputCommandType::FGameplayInputCommandType(const FGameplayTag InputTag, const EGameplayInputType InputType)
{
	this->InputTag = InputTag;
	this->InputType = InputType;
}

bool FGameplayInputCommandType::operator==(const FGameplayInputCommandType& Other) const
{
	return InputTag == Other.InputTag && InputType == Other.InputType;
}

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
