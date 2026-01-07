// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputSystemEnums.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GameplayInputForwardingMapping.generated.h"

USTRUCT(BlueprintType)
struct FGameplayInputForwardingEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag InputTag;

	UPROPERTY(EditAnywhere)
	EGameplayInputType InputType;
	
	// GetTypeHash
	friend uint32 GetTypeHash(const FGameplayInputForwardingEntry& Entry)
	{
		return HashCombine(GetTypeHash(Entry.InputTag), GetTypeHash(static_cast<uint8>(Entry.InputType)));
	}
};

/**
 * 
 */
UCLASS()
class GAMEPLAYINPUTSYSTEM_API UGameplayInputForwardingMapping : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding")
	TMap<FGameplayInputForwardingEntry, FGameplayTag> ForwardingMap;
};
