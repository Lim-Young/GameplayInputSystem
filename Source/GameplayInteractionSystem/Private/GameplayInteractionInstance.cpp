// Copyright Lim Young.


#include "GameplayInteractionInstance.h"

#include "GameplayInteractionProcessor.h"
#include "GameplayInteractionWidgetInterface.h"
#include "Blueprint/UserWidget.h"

bool FGameplayInteractionWidgetConfig::Valid() const
{
	return IsValid(InteractionWidgetClass);
}

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

	// Initialize the interaction widget
	if (!InteractionDescription.WidgetConfig.Valid())
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("UGameplayInteractionInstance::InitializeInteractionInstance - Invalid WidgetConfig"));
		return;
	}

	InteractionWidget = CreateWidget(GetWorld(), InteractionDescription.WidgetConfig.InteractionWidgetClass);
	if (!InteractionWidget)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("UGameplayInteractionInstance::InitializeInteractionInstance - Failed to create InteractionWidget"
		       ));
		return;
	}

	InteractionWidget->AddToViewport();
	InteractionWidget->SetDesiredSizeInViewport(InteractionDescription.WidgetConfig.WidgetDesiredSize);
	InteractionWidget->SetAnchorsInViewport(InteractionDescription.WidgetConfig.WidgetAnchors);
	InteractionWidget->SetAlignmentInViewport(InteractionDescription.WidgetConfig.WidgetAlignment);
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

void UGameplayInteractionInstance::ProcessGameplayInputEvent(const FGameplayTag& InputTag,
                                                             const EGameplayInputType InputType) const
{
	OnInteractionInputEvent.Broadcast(InputTag, InputType);

	if (!IsValid(InteractionWidget))
	{
		return;
	}

	IGameplayInteractionWidgetInterface::Execute_OnInteractionInputEvent(InteractionWidget, InputTag, InputType);
}

void UGameplayInteractionInstance::CompleteInteraction(const bool bSuccess)
{
	if (bSuccess)
	{
		OnInteractionSuccess.Broadcast();
		if (IsValid(InteractionWidget))
		{
			IGameplayInteractionWidgetInterface::Execute_OnInteractionSuccess(InteractionWidget);
		}
	}
	else
	{
		OnInteractionFailed.Broadcast();
		if (IsValid(InteractionWidget))
		{
			IGameplayInteractionWidgetInterface::Execute_OnInteractionFailed(InteractionWidget);
		}
	}

	OnInteractionComplete.Broadcast();
	if (IsValid(InteractionWidget))
	{
		IGameplayInteractionWidgetInterface::Execute_OnInteractionComplete(InteractionWidget);
	}

	Cleanup();

	MarkAsGarbage();
}
