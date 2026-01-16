// Copyright Lim Young.


#include "GameplayInputActionEvent.h"

FGameplayInputActionEvent::FGameplayInputActionEvent()
{
	InputActionTag = FGameplayTag::EmptyTag;
	InputActionState = EGameplayInputActionState::Idle;
}

FGameplayInputActionEvent::FGameplayInputActionEvent(const FGameplayTag InputSourceTag)
{
	InputActionTag = InputSourceTag;
	InputActionState = EGameplayInputActionState::Idle;
}

FGameplayInputActionEvent::FGameplayInputActionEvent(const FGameplayTag InputSourceTag,
                                                     EGameplayInputActionState InInputActionState)
{
	InputActionTag = InputSourceTag;
	InputActionState = InInputActionState;
}

bool FGameplayInputActionEvent::operator==(const FGameplayInputActionEvent& Other) const
{
	return InputActionTag == Other.InputActionTag && InputActionState == Other.InputActionState;
}

void UGameplayInputActionEventInstance::Initialize(const FGameplayTag& InInputActionTag,
                                                   const EGameplayInputActionState InInputActionState, uint8 InPriority,
                                                   float InLifetime, float InTimestamp)
{
	InputActionTag = InInputActionTag;
	InputActionState = InInputActionState;
	Priority = InPriority;
	Lifetime = InLifetime;

	Timestamp = InTimestamp;
}

void UGameplayInputActionEventInstance::Initialize(const FGameplayTag& InInputActionTag,
                                                   const EGameplayInputActionState InInputActionState,
                                                   const FGameplayInputActionEventConfig& InConfig)
{
	Initialize(InInputActionTag, InInputActionState, InConfig.Priority, InConfig.Lifetime,
	           GetWorld()->GetTimeSeconds());
}
