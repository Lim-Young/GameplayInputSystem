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

private:
	UPROPERTY(Transient)
	TObjectPtr<UGameplayInputSubsystem> GameplayInputSubsystem;

	UPROPERTY(Transient)
	TObjectPtr<UGameplayInteractionInstance> OwnerInteractionInstance;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Interaction Processor")
	void OnGameplayInputEvent(FGameplayTag InputTag, EGameplayInputType InputType);

public:
	void Initialize(UGameplayInteractionInstance* InteractionInstance);
	void Cleanup();

protected:
	UFUNCTION(BlueprintCallable, Category = "Gameplay Interaction Processor")
	void CompleteInteraction(const bool bSuccess) const;
};
