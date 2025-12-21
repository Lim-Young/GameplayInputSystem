// Copyright (c) Lim Young

#pragma once

#include "CoreMinimal.h"
#include "GameplayInteractionInstance.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "GameplayInteractableComponent.generated.h"

class UGameplayInteractorComponent;

UENUM()
enum class EGameplayInteractionState : uint8
{
	Inactive,
	Active,
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYINTERACTIONSYSTEM_API UGameplayInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGameplayInteractableComponent();
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

// private:
// 	UFUNCTION()
// 	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
// 	                             const FHitResult& SweepResult);
// 	UFUNCTION()
// 	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Interaction")
	FGameplayTagContainer RequiredInteractorTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Interaction")
	FGameplayTagContainer IgnoredInteractorTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Interaction")
	FGameplayInteractionDescription InteractionDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Interaction")
	bool bCanActive = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Interaction|Widget")
	FName InteractionWidgetName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Interaction|Collision")
	TArray<FName> InteractableCollisionComponentTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Interaction|Highlight")
	TObjectPtr<UMaterialInterface> HighlightMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Interaction|Highlight")
	TArray<FName> HighlightMeshComponentTags;

	UPROPERTY(BlueprintAssignable, Category = "Gameplay Interaction|Events")
	FOnInteractionComplete OnInteractionSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Gameplay Interaction|Events")
	FOnInteractionFailed OnInteractionFailed;

	UPROPERTY(BlueprintAssignable, Category = "Gameplay Interaction|Events")
	FOnInteractionComplete OnInteractionComplete;

	UPROPERTY(BlueprintAssignable, Category = "Gameplay Interaction|Events")
	FOnInteractionInputEvent OnInteractionInputEvent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Interaction|Read Only")
	EGameplayInteractionState InteractionState = EGameplayInteractionState::Inactive;

	UPROPERTY()
	TObjectPtr<UWidgetComponent> InteractionWidgetComponent;

	TArray<TObjectPtr<UPrimitiveComponent>> CollisionComponents;

	UPROPERTY()
	TArray<TObjectPtr<UMeshComponent>> HighlightMeshComponents;

private:
	UPROPERTY()
	TObjectPtr<UGameplayInteractorComponent> CurrentInteractor;

	UPROPERTY()
	UGameplayInteractionInstance* GameplayInteractionInstance;

public:
	bool CheckInteractorTags(const FGameplayTagContainer& InteractorTags) const;
	bool ActiveInteraction(UGameplayInteractorComponent* InteractorComponent);
	bool DeactivateInteraction();

private:
	UFUNCTION()
	void ProcessInteractionInputEvent(const FGameplayTag& InputTag, const EGameplayInputType InputType);
	UFUNCTION()
	void ProcessInteractionSuccess();
	UFUNCTION()
	void ProcessInteractionFailed();
	UFUNCTION()
	void ProcessInteractionComplete();
};
