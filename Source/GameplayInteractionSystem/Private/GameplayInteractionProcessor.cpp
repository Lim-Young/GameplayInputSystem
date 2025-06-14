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

void UGameplayInteractionProcessor::Initialize(UGameplayInteractionInstance* InteractionInstance)
{
	GameplayInputSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameplayInputSubsystem>();
	OwnerInteractionInstance = InteractionInstance;

	GameplayInputSubsystem->OnGameplayInputEvent.AddDynamic(
		this, &UGameplayInteractionProcessor::ProcessGameplayInputEvent);
}

void UGameplayInteractionProcessor::Cleanup()
{
	if (IsValid(GameplayInputSubsystem))
	{
		GameplayInputSubsystem->OnGameplayInputEvent.RemoveDynamic(
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
		UE_LOG(LogTemp, Error,
		       TEXT("UGameplayInteractionProcessor::CompleteInteraction - OwnerInteractionInstance is invalid"));
	}
}
