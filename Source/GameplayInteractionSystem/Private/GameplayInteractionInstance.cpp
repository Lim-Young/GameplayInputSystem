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

void UGameplayInteractionInstance::Cleanup()
{
	// Clean up the interaction instance
	OnInteractionSuccess.Clear();
	OnInteractionFailed.Clear();
	OnInteractionComplete.Clear();
}

void UGameplayInteractionInstance::CompleteInteraction(const bool bSuccess)
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

	Cleanup();

	MarkAsGarbage();
}
