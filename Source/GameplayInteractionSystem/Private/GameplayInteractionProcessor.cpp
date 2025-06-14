// Copyright Lim Young.


#include "GameplayInteractionProcessor.h"

void UGameplayInteractionProcessor::OnGameplayInputEvent_Implementation(FGameplayTag InputTag,
	EGameplayInputType InputType)
{
}

void UGameplayInteractionProcessor::Initialize(UGameplayInteractionInstance* InteractionInstance)
{
	GameplayInputSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameplayInputSubsystem>();
	OwnerInteractionInstance = InteractionInstance;

	GameplayInputSubsystem->OnGameplayInputEvent.AddDynamic(
		this, &UGameplayInteractionProcessor::OnGameplayInputEvent);
}

void UGameplayInteractionProcessor::Cleanup()
{
	if (IsValid(GameplayInputSubsystem))
	{
		GameplayInputSubsystem->OnGameplayInputEvent.RemoveDynamic(
			this, &UGameplayInteractionProcessor::OnGameplayInputEvent);
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
		UE_LOG(LogTemp, Error, TEXT("UGameplayInteractionProcessor::CompleteInteraction - OwnerInteractionInstance is invalid"));
	}
}
