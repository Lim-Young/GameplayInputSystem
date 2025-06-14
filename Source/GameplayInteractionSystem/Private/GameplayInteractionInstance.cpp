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
		UE_LOG(LogTemp, Warning,
		       TEXT("UGameplayInteractionInstance::InitializeInteractionInstance - Invalid InteractionDescription"));
		return;
	}

	InteractionProcessor = DuplicateObject(InteractionDescription.InteractionProcessor, this);
	InteractionProcessor->Initialize(this);

	// Avoid garbage collection issues by cleaning up the instance when the world is cleaned up
	FWorldDelegates::OnWorldCleanup.AddUObject(this, &UGameplayInteractionInstance::OnWorldCleanup);
}

void UGameplayInteractionInstance::Cleanup()
{
	// Clean up the interaction instance
	OnInteractionSuccess.Clear();
	OnInteractionFailed.Clear();
	OnInteractionComplete.Clear();

	InteractionProcessor->Cleanup();

	FWorldDelegates::OnWorldCleanup.RemoveAll(this);
	RemoveFromRoot();
}

void UGameplayInteractionInstance::OnWorldCleanup(UWorld* World, bool bArg, bool bCond)
{
	if (World != GetWorld())
	{
		return;
	}

	// Clean up the interaction instance
	Cleanup();
}
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
