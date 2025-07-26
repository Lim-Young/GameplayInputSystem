// Copyright (c) Lim Young


#include "GameplayInteractorProcessor.h"

#include "Component/GameplayInteractorComponent.h"

void UGameplayInteractorProcessor::Initialize(UGameplayInteractorComponent* GameplayInteractorComponent)
{
	OwnerInteractorComponent = GameplayInteractorComponent;
	check(OwnerInteractorComponent);

	OwnerActor = OwnerInteractorComponent->GetOwner();

	PostInitialize();
}

void UGameplayInteractorProcessor::PostInitialize_Implementation()
{
}

void UGameplayInteractorProcessor::Tick_Implementation(float DeltaTime)
{
}

void UGameplayInteractorProcessor::Cleanup_Implementation()
{
}

void UGameplayInteractorProcessor::ActiveInteractableComponent(
	UGameplayInteractableComponent* InteractableComponent) const
{
	if (!IsValid(InteractableComponent))
	{
		return;
	}

	if (InteractableComponent->ActiveInteraction(OwnerInteractorComponent))
	{
		OwnerInteractorComponent->AddInteractableComponent(InteractableComponent);
	}
}

void UGameplayInteractorProcessor::DeactivateInteractableComponent(
	UGameplayInteractableComponent* InteractableComponent) const
{
	if (!IsValid(InteractableComponent))
	{
		return;
	}

	if (InteractableComponent->DeactivateInteraction())
	{
		OwnerInteractorComponent->RemoveInteractableComponent(InteractableComponent);
	}
}
