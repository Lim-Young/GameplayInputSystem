// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayInputCommand.h"

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
