// Copyright (c) Lim Young

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayInteractorProcessor.generated.h"

class UGameplayInteractorComponent;

/**
 * 
 */
UCLASS(Abstract, DefaultToInstanced, CollapseCategories, EditInlineNew)
class GAMEPLAYINTERACTIONSYSTEM_API UGameplayInteractorProcessor : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay Interaction")
	bool bTickable = false;

protected:
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;

	UPROPERTY()
	TObjectPtr<UGameplayInteractorComponent> OwnerInteractorComponent;

public:
	void Initialize(UGameplayInteractorComponent* GameplayInteractorComponent);

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Interaction")
	void PostInitialize();

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Interaction")
	void Tick(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Interaction")
	void Cleanup();

protected:
	UFUNCTION(BlueprintCallable, Category = "Gameplay Interaction")
	void ActiveInteractableComponent(UGameplayInteractableComponent* InteractableComponent) const;

	UFUNCTION(BlueprintCallable, Category = "Gameplay Interaction")
	void DeactivateInteractableComponent(UGameplayInteractableComponent* InteractableComponent) const;
};
