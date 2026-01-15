// Copyright Lim Young.


#include "GameplayInputCommand.h"

FGameplayInputCommand::FGameplayInputCommand()
{
	InputSourceTag = FGameplayTag::EmptyTag;
	InputType = EGameplayInputType::Pressed;
}

FGameplayInputCommand::FGameplayInputCommand(const FGameplayTag InputSourceTag, const EGameplayInputType InputType)
{
	this->InputSourceTag = InputSourceTag;
	this->InputType = InputType;
}

bool FGameplayInputCommand::operator==(const FGameplayInputCommand& Other) const
{
	return InputSourceTag == Other.InputSourceTag && InputType == Other.InputType;
}

void UGameplayInputCommandInstance::Initialize(const FGameplayTag& InInputSourceTag, const EGameplayInputType InInputType,
                                               uint8 InPriority, float InLifetime, float InTimestamp)
{
	InputSourceTag = InInputSourceTag;
	InputType = InInputType;
	Priority = InPriority;
	Lifetime = InLifetime;

	Timestamp = InTimestamp;
}

void UGameplayInputCommandInstance::Initialize(const FGameplayTag& InInputSourceTag, const EGameplayInputType InInputType,
                                               const FGameplayInputCommandConfig& InConfig)
{
	Initialize(InInputSourceTag, InInputType, InConfig.Priority, InConfig.Lifetime, GetWorld()->GetTimeSeconds());
}
