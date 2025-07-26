// Copyright (c) Lim Young


#include "Component/GameplayInteractableComponent.h"

#include "GameplayInteractionSystemLibrary.h"
#include "Component/GameplayInteractorComponent.h"


// Sets default values for this component's properties
UGameplayInteractableComponent::UGameplayInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UGameplayInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!InteractionDescription.Valid())
	{
		return;
	}

	if (!InteractionWidgetName.IsNone())
	{
		TArray<UWidgetComponent*> WidgetComponents;
		GetOwner()->GetComponents<UWidgetComponent>(WidgetComponents);
		auto ResultWidgetComponent = WidgetComponents.FindByPredicate(
			[&](const UWidgetComponent* WidgetComponent)
			{
				return WidgetComponent && WidgetComponent->GetFName() == InteractionWidgetName;
			});

		if (ResultWidgetComponent)
		{
			InteractionWidgetComponent = *ResultWidgetComponent;
		}
	}
	//
	// if (InteractableCollisionComponentTags.Num() > 0)
	// {
	// 	GetOwner()->GetComponents<UPrimitiveComponent>(CollisionComponents);
	// 	for (UPrimitiveComponent* CollisionComponent : CollisionComponents)
	// 	{
	// 		check(CollisionComponent);
	// 		CollisionComponent->OnComponentBeginOverlap.AddDynamic(
	// 			this, &UGameplayInteractableComponent::OnComponentBeginOverlap);
	// 		CollisionComponent->OnComponentEndOverlap.AddDynamic(
	// 			this, &UGameplayInteractableComponent::OnComponentEndOverlap);
	// 	}
	// }
}

void UGameplayInteractableComponent::BeginDestroy()
{
	Super::BeginDestroy();

	// if (CollisionComponents.Num() > 0)
	// {
	// 	for (UPrimitiveComponent* CollisionComponent : CollisionComponents)
	// 	{
	// 		if (!IsValid(CollisionComponent))
	// 		{
	// 			continue;
	// 		}
	//
	// 		CollisionComponent->OnComponentBeginOverlap.RemoveDynamic(
	// 			this, &UGameplayInteractableComponent::OnComponentBeginOverlap);
	// 		CollisionComponent->OnComponentEndOverlap.RemoveDynamic(
	// 			this, &UGameplayInteractableComponent::OnComponentEndOverlap);
	// 	}
	// 	CollisionComponents.Empty();
	// }
}

// void UGameplayInteractableComponent::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,
//                                                              AActor* OtherActor, UPrimitiveComponent* OtherComp,
//                                                              int32 OtherBodyIndex, bool bFromSweep,
//                                                              const FHitResult& SweepResult)
// {
// 	if (!IsValid(OtherActor))
// 	{
// 		return;
// 	}
//
// 	UGameplayInteractorComponent* Interactor = OtherActor->FindComponentByClass<UGameplayInteractorComponent>();
// 	if (!IsValid(Interactor))
// 	{
// 		return;
// 	}
//
// 	if (CheckInteractorTags(Interactor->InteractorTags))
// 	{
// 		CurrentInteractor = Interactor;
// 		ActiveInteraction(Interactor);
// 	}
// }
//
// void UGameplayInteractableComponent::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
//                                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	if (!IsValid(OtherActor))
// 	{
// 		return;
// 	}
//
// 	UGameplayInteractorComponent* Interactor = OtherActor->FindComponentByClass<UGameplayInteractorComponent>();
// 	if (!IsValid(Interactor))
// 	{
// 		return;
// 	}
//
// 	DeactivateInteraction();
// }

bool UGameplayInteractableComponent::CheckInteractorTags(const FGameplayTagContainer& InteractorTags) const
{
	if (RequiredInteractorTags.Num() > 0)
	{
		if (!RequiredInteractorTags.HasAnyExact(InteractorTags))
		{
			return false;
		}
	}

	if (IgnoredInteractorTags.Num() > 0)
	{
		if (IgnoredInteractorTags.HasAnyExact(InteractorTags))
		{
			return false;
		}
	}

	return true;
}

bool UGameplayInteractableComponent::ActiveInteraction(UGameplayInteractorComponent* InteractorComponent)
{
	if (!bCanActive)
	{
		return false;
	}

	if (!IsValid(InteractorComponent))
	{
		return false;
	}

	if (!InteractionDescription.Valid())
	{
		return false;
	}

	CurrentInteractor = InteractorComponent;

	GameplayInteractionInstance = UGameplayInteractionSystemLibrary::CreateGameplayInteractionInstance(
		this, InteractionDescription, InteractionWidgetComponent);

	if (IsValid(GameplayInteractionInstance))
	{
		InteractionState = EGameplayInteractionState::Active;

		GameplayInteractionInstance->OnInteractionInputEvent.AddDynamic(
			this, &UGameplayInteractableComponent::ProcessInteractionInputEvent);
		GameplayInteractionInstance->OnInteractionSuccess.AddDynamic(
			this, &UGameplayInteractableComponent::ProcessInteractionSuccess);
		GameplayInteractionInstance->OnInteractionFailed.AddDynamic(
			this, &UGameplayInteractableComponent::ProcessInteractionFailed);
		GameplayInteractionInstance->OnInteractionComplete.AddDynamic(
			this, &UGameplayInteractableComponent::ProcessInteractionComplete);

		return true;
	}

	return false;
}

bool UGameplayInteractableComponent::DeactivateInteraction()
{
	if (InteractionState == EGameplayInteractionState::Inactive)
	{
		return true; // Already inactive, nothing to do
	}

	if (!IsValid(GameplayInteractionInstance))
	{
		InteractionState = EGameplayInteractionState::Inactive;
		return true;
	}

	GameplayInteractionInstance->DestroyInteractionInstance();

	InteractionState = EGameplayInteractionState::Inactive;

	CurrentInteractor = nullptr;
	GameplayInteractionInstance = nullptr;
	return true;
}

void UGameplayInteractableComponent::ProcessInteractionInputEvent(const FGameplayTag& InputTag,
                                                                  const EGameplayInputType InputType)
{
	OnInteractionInputEvent.Broadcast(InputTag, InputType);
}

void UGameplayInteractableComponent::ProcessInteractionSuccess()
{
	OnInteractionSuccess.Broadcast();
}

void UGameplayInteractableComponent::ProcessInteractionFailed()
{
	OnInteractionFailed.Broadcast();
}

void UGameplayInteractableComponent::ProcessInteractionComplete()
{
	OnInteractionComplete.Broadcast();
}
