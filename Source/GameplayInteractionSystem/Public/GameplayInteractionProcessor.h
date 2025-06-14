// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputSubsystem.h"
#include "GameplayInteractionInstance.h"
#include "UObject/Object.h"
#include "GameplayInteractionProcessor.generated.h"

/**
 * 
 */
UCLASS(Abstract, DefaultToInstanced, CollapseCategories, EditInlineNew)
class GAMEPLAYINTERACTIONSYSTEM_API UGameplayInteractionProcessor : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<UWorld> OwnerWorld;

private:
	UPROPERTY(Transient)
	TObjectPtr<UGameplayInputSubsystem> GameplayInputSubsystem;

	UPROPERTY(Transient)
	TObjectPtr<UGameplayInteractionInstance> OwnerInteractionInstance;

	UFUNCTION()
	void ProcessGameplayInputEvent(const FGameplayTag& InputTag, EGameplayInputType InputType);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Interaction Processor")
	void OnGameplayInputEvent(const FGameplayTag& InputTag, const EGameplayInputType InputType);

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Interaction Processor")
	void PostInitialize();

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Interaction Processor")
	void PreCleanup();

public:
	void Initialize(UGameplayInteractionInstance* InteractionInstance);
	void Cleanup();

protected:
	UFUNCTION(BlueprintCallable, Category = "Gameplay Interaction Processor")
	void CompleteInteraction(const bool bSuccess) const;
};
