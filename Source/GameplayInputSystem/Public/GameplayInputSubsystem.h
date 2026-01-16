// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputBuffer.h"
#include "GameplayInputSystemEnums.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayInputSubsystem.generated.h"

class UGameplayInputActionSet;

/**
 * 
 */
UCLASS()
class GAMEPLAYINPUTSYSTEM_API UGameplayInputSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override
	{
		return Outer->GetWorld()->GetNetMode() != NM_DedicatedServer;
	}

private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameplayInputSourceEvent, const FGameplayTag&, InputSourceTag,
	                                             const EGameplayInputType, InputType);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameplayInputActionTriggered, const FGameplayTag&, InputActionTag,
	                                             const EGameplayInputActionState, ActionState);

public:
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Input System|Events")
	FOnGameplayInputSourceEvent OnGameplayInputSourceEvent;

	UPROPERTY(BlueprintAssignable, Category = "Gameplay Input System|Events")
	FOnGameplayInputActionTriggered OnGameplayInputActionTriggered;

private:
	UPROPERTY()
	TMap<TObjectPtr<UGameplayInputBufferSchema>, TObjectPtr<UGameplayInputBuffer>> GameplayInputBuffers;

	UPROPERTY()
	TArray<TObjectPtr<UGameplayInputActionSet>> ActiveGameplayInputActionSets;

	UPROPERTY()
	TMap<TObjectPtr<UGameplayInputActionSet>, TObjectPtr<UGameplayInputActionSet>> GameplayInputActionSetMapping;

public:
	void InjectGameplayInput(const FGameplayTag& InputSourceTag, const EGameplayInputType InputType);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Input System")
	void CreateAndRegisterGameplayInputBuffer(UGameplayInputBufferSchema* InGameplayInputDocker,
	                                          EGameplayInputBufferScope InputBufferScope);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Input System")
	void FinishAndUnregisterGameplayInputBuffer(UGameplayInputBufferSchema* InGameplayInputDocker);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Input System")
	void AddGameplayInputActionSet(UGameplayInputActionSet* InInputActionSet, uint8 Priority = 0);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Input System")
	void RemoveGameplayInputActionSet(UGameplayInputActionSet* InInputActionSet);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Input System")
	void ForceTriggerGameplayInputAction(const FGameplayTag& InputActionTag, EGameplayInputActionState ActionState);

private:
	void BroadcastGameplayInputEvent(const FGameplayTag& InputSourceTag, EGameplayInputType InputType) const;
	void BroadcastGameplayInputActionTriggered(const FGameplayTag& InputActionTag,
	                                           const EGameplayInputActionState ActionState) const;
};
