// Copyright (c) Lim Young


#include "Component/GameplayInteractorComponent.h"

UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayInteraction_Interactor, "GameplayInteraction.Interactor",
                               "Tag used to identify gameplay interaction interactor components. This tag is used to differentiate between interactor components and other types of components.")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayInteraction_Interactor_Player, "GameplayInteraction.Interactor.Player",
                               "Tag used to identify player interactor components. This tag is used to differentiate between player interactor components and other types of interactor components.")

UGameplayInteractorComponent::UGameplayInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGameplayInteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	ActiveInteractableComponents.Empty();

	ParallelFor(Processors.Num(), [&](int32 Index)
	{
		if (UGameplayInteractorProcessor* Processor = Processors[Index])
		{
			Processor->Initialize(this);
		}
	});
}

// Called every frame
void UGameplayInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ParallelFor(Processors.Num(), [&](int32 Index)
	{
		if (UGameplayInteractorProcessor* Processor = Processors[Index])
		{
			if (Processor->bTickable)
			{
				Processor->Tick(DeltaTime);
			}
		}
	});
}

void UGameplayInteractorComponent::BeginDestroy()
{
	Super::BeginDestroy();

	ParallelFor(Processors.Num(), [&](int32 Index)
	{
		if (UGameplayInteractorProcessor* Processor = Processors[Index])
		{
			Processor->Cleanup();
		}
	});
}

void UGameplayInteractorComponent::AddInteractableComponent(UGameplayInteractableComponent* InteractableComponent)
{
	if (ActiveInteractableComponents.Contains(InteractableComponent))
	{
		return;
	}

	ActiveInteractableComponents.Add(InteractableComponent);

	OnInteractableComponentAdded.Broadcast(InteractableComponent);
}

void UGameplayInteractorComponent::RemoveInteractableComponent(UGameplayInteractableComponent* InteractableComponent)
{
	if (!ActiveInteractableComponents.Contains(InteractableComponent))
	{
		return;
	}

	PreInteractableComponentRemove.Broadcast(InteractableComponent);

	ActiveInteractableComponents.Remove(InteractableComponent);
}
