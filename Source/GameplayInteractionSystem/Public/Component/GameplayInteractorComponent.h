// Copyright (c) Lim Young

#pragma once

#include "CoreMinimal.h"
#include "GameplayInteractableComponent.h"
#include "GameplayInteractorProcessor.h"
#include "NativeGameplayTags.h"
#include "Components/ActorComponent.h"
#include "GameplayInteractorComponent.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayInteraction_Interactor);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayInteraction_Interactor_Player);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableComponentAdded, UGameplayInteractableComponent*,
                                            InteractableComponent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPreInteractableComponentRemove, UGameplayInteractableComponent*,
                                            InteractableComponent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYINTERACTIONSYSTEM_API UGameplayInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGameplayInteractorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginDestroy() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Interaction", Instanced)
	TArray<TObjectPtr<UGameplayInteractorProcessor>> Processors;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer InteractorTags;

private:
	TArray<TObjectPtr<UGameplayInteractableComponent>> ActiveInteractableComponents;

public:
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Interaction")
	FOnInteractableComponentAdded OnInteractableComponentAdded;

	UPROPERTY(BlueprintAssignable, Category = "Gameplay Interaction")
	FPreInteractableComponentRemove PreInteractableComponentRemove;

	void AddInteractableComponent(UGameplayInteractableComponent* InteractableComponent);
	void RemoveInteractableComponent(UGameplayInteractableComponent* InteractableComponent);
};
