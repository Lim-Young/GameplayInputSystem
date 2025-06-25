// Copyright (c) Lim Young


#include "InteractorProcessor/GameplayInteractorProcessor_Overlap.h"

#include "Component/GameplayInteractorComponent.h"

void UGameplayInteractorProcessor_Overlap::PostInitialize_Implementation()
{
	if (OverlapComponentTags.Num() <= 0)
	{
		return;
	}

	for (const FName Tag : OverlapComponentTags)
	{
		TArray<UActorComponent*> OverlapComponents = OwnerActor->GetComponentsByTag(
			UPrimitiveComponent::StaticClass(), Tag);

		for (UActorComponent* Component : OverlapComponents)
		{
			if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Component))
			{
				if (ActiveOverlapComponents.Contains(PrimitiveComponent))
				{
					return;
				}

				PrimitiveComponent->OnComponentBeginOverlap.AddDynamic(
					this, &UGameplayInteractorProcessor_Overlap::OnOverlapBegin);
				PrimitiveComponent->OnComponentEndOverlap.AddDynamic(
					this, &UGameplayInteractorProcessor_Overlap::OnOverlapEnd);

				ActiveOverlapComponents.AddUnique(PrimitiveComponent);
			}
		}
	}
}

void UGameplayInteractorProcessor_Overlap::Cleanup_Implementation()
{
	if (ActiveOverlapComponents.Num() <= 0)
	{
		return;
	}

	for (UPrimitiveComponent* PrimitiveComponent : ActiveOverlapComponents)
	{
		if (IsValid(PrimitiveComponent))
		{
			PrimitiveComponent->OnComponentBeginOverlap.RemoveDynamic(
				this, &UGameplayInteractorProcessor_Overlap::OnOverlapBegin);
			PrimitiveComponent->OnComponentEndOverlap.RemoveDynamic(
				this, &UGameplayInteractorProcessor_Overlap::OnOverlapEnd);
		}
	}

	ActiveOverlapComponents.Empty();
}

void UGameplayInteractorProcessor_Overlap::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                          bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	TArray<UGameplayInteractableComponent*> InteractableComponents;
	OtherActor->GetComponents<UGameplayInteractableComponent>(InteractableComponents);

	for (const auto InteractableComponent : InteractableComponents)
	{
		// TODO: Add Priority Or Predicate Condition

		if (InteractableComponent->CheckInteractorTags(OwnerInteractorComponent->InteractorTags))
		{
			ActiveInteractableComponent(InteractableComponent);
			return;
		}
	}
}

void UGameplayInteractorProcessor_Overlap::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	TArray<UGameplayInteractableComponent*> InteractableComponents;
	OtherActor->GetComponents<UGameplayInteractableComponent>(InteractableComponents);

	for (const auto InteractableComponent : InteractableComponents)
	{
		if (InteractableComponent->CheckInteractorTags(OwnerInteractorComponent->InteractorTags))
		{
			DeactivateInteractableComponent(InteractableComponent);
			return;
		}
	}
}
