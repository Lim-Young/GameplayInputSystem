// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputArbiter.h"
#include "GameplayInputStructsEnums.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayInputSubsystem.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayInput)

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
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameplayInputEvent, const FGameplayTag&, InputTag,
	                                             const EGameplayInputType, InputType);

public:
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Input System|Events")
	FOnGameplayInputEvent OnGameplayInputEvent;

private:
	UPROPERTY()
	TMap<TObjectPtr<UGameplayInputDocket>, TObjectPtr<UGameplayInputArbiter>> GameplayInputArbiters;

public:
	void InjectGameplayInput(const FGameplayTag& InputTag, const EGameplayInputType InputType);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Input System")
	void CreateAndRegisterGameplayInputArbiter(UGameplayInputDocket* InGameplayInputDocker);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Input System")
	void FinishAndUnregisterGameplayInputArbiter(UGameplayInputDocket* InGameplayInputDocker);
};
