// Copyright Lim Young.


#include "GameplayInputActionEvent.h"

FGameplayInputActionEvent::FGameplayInputActionEvent()
{
	InputActionTag = FGameplayTag::EmptyTag;
	InputActionEvent = EGameplayInputActionEvent::None;
}

FGameplayInputActionEvent::FGameplayInputActionEvent(const FGameplayTag InputSourceTag)
{
	InputActionTag = InputSourceTag;
	InputActionEvent = EGameplayInputActionEvent::None;
}

FGameplayInputActionEvent::FGameplayInputActionEvent(const FGameplayTag InputSourceTag,
                                                     EGameplayInputActionEvent InInputActionEvent)
{
	InputActionTag = InputSourceTag;
	InputActionEvent = InInputActionEvent;
}

bool FGameplayInputActionEvent::operator==(const FGameplayInputActionEvent& Other) const
{
	return InputActionTag == Other.InputActionTag && InputActionEvent == Other.InputActionEvent;
}

void UGameplayInputActionEventInstance::Initialize(const FGameplayTag& InInputActionTag,
                                                   const EGameplayInputActionEvent InInputActionEvent, uint8 InPriority,
                                                   float InLifetime, float InTimestamp)
{
	InputActionTag = InInputActionTag;
	InputActionEvent = InInputActionEvent;
	Priority = InPriority;
	Lifetime = InLifetime;

	Timestamp = InTimestamp;
}

void UGameplayInputActionEventInstance::Initialize(const FGameplayTag& InInputActionTag,
                                                   const EGameplayInputActionEvent InInputActionEvent,
                                                   const FGameplayInputActionEventConfig& InConfig)
{
	Initialize(InInputActionTag, InInputActionEvent, InConfig.Priority, InConfig.Lifetime,
	           GetWorld()->GetTimeSeconds());
}
