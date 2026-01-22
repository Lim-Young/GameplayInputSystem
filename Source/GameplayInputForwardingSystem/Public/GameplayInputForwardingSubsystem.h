// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputForwardingMapping.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayInputForwardingSubsystem.generated.h"

class UGameplayInputSubsystem;

/**
 * 
 */
UCLASS()
class GAMEPLAYINPUTFORWARDINGSYSTEM_API UGameplayInputForwardingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override
	{
		return Outer->GetWorld()->GetNetMode() != NM_DedicatedServer;
	}

	virtual void PostInitialize() override;

protected:
	UFUNCTION()
	void HandleGameplayInputSourceEvent(const FGameplayTag& InputTag, const EGameplayInputType InputType);

	UFUNCTION()
	void HandleGameplayInputActionEvent(const FGameplayTag& InputActionTag,
	                                    const EGameplayInputActionEvent ActionState);

private:
	UPROPERTY()
	TObjectPtr<UGameplayInputSubsystem> GameplayInputSubsystem;

	UPROPERTY()
	TArray<UGameplayInputForwardingMapping*> GameplayInputForwardingMappings;

	UPROPERTY()
	TObjectPtr<AActor> CachedLocalPlayerPawn;

public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay Input System")
	void AddGameplayInputForwardingMapping(UGameplayInputForwardingMapping* InMapping);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Input System")
	void RemoveGameplayInputForwardingMapping(UGameplayInputForwardingMapping* InMapping);

protected:
	bool TryGetInputForwardingMapping(UGameplayInputForwardingMapping*& OutMapping) const;
};
