// Copyright Lim Young.


#include "GameplayInteractionInstance.h"

#include "GameplayInteractionProcessor.h"

bool FGameplayInteractionDescription::Valid() const
{
	return IsValid(InteractionProcessor);
}

void UGameplayInteractionInstance::BeginDestroy()
{
	UObject::BeginDestroy();
}

void UGameplayInteractionInstance::InitializeInteractionInstance(
	const FGameplayInteractionDescription& InteractionDescription)
{
	if (!InteractionDescription.Valid())
	{
		UE_LOG(LogTemp, Error,
		       TEXT("UGameplayInteractionInstance::InitializeInteractionInstance - Invalid InteractionDescription"));
		return;
	}

	InteractionProcessor = DuplicateObject(InteractionDescription.InteractionProcessor, this);
	InteractionProcessor->Initialize(this);
}

void UGameplayInteractionInstance::CompleteInteraction(const bool bSuccess) const
{
	if (bSuccess)
	{
		OnInteractionSuccess.Broadcast();
	}
	else
	{
		OnInteractionFailed.Broadcast();
	}

	OnInteractionComplete.Broadcast();
}
