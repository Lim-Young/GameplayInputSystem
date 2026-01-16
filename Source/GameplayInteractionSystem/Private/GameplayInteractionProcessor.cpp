// Copyright Lim Young.


#include "GameplayInteractionProcessor.h"

void UGameplayInteractionProcessor::ProcessGameplayInputEvent(const FGameplayTag& InputTag,
                                                              const EGameplayInputType InputType)
{
	OwnerInteractionInstance->ProcessGameplayInputEvent(InputTag, InputType);

	OnGameplayInputEvent(InputTag, InputType);
}

void UGameplayInteractionProcessor::OnGameplayInputEvent_Implementation(const FGameplayTag& InputTag,
                                                                        const EGameplayInputType InputType)
{
}

void UGameplayInteractionProcessor::PostInitialize_Implementation()
{
}

void UGameplayInteractionProcessor::PreCleanup_Implementation()
{
}

void UGameplayInteractionProcessor::Initialize(UGameplayInteractionInstance* InteractionInstance)
{
	OwnerWorld = GetWorld();
	ensureMsgf(OwnerWorld, TEXT("UGameplayInteractionProcessor::Initialize - OwnerWorld is invalid"));

	GameplayInputSubsystem = GetWorld()->GetSubsystem<UGameplayInputSubsystem>();
	OwnerInteractionInstance = InteractionInstance;

	GameplayInputSubsystem->OnGameplayInputSourceEvent.AddDynamic(
		this, &UGameplayInteractionProcessor::ProcessGameplayInputEvent);

	PostInitialize();
}

void UGameplayInteractionProcessor::Cleanup()
{
	PreCleanup();

	if (IsValid(GameplayInputSubsystem))
	{
		GameplayInputSubsystem->OnGameplayInputSourceEvent.RemoveDynamic(
			this, &UGameplayInteractionProcessor::ProcessGameplayInputEvent);
	}
}

void UGameplayInteractionProcessor::CompleteInteraction(const bool bSuccess) const
{
	if (IsValid(OwnerInteractionInstance))
	{
		OwnerInteractionInstance->CompleteInteraction(bSuccess);
	}
	else
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("UGameplayInteractionProcessor::CompleteInteraction - OwnerInteractionInstance is invalid"));
	}
}
