// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayInputSystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYINPUTSYSTEM_API UGameplayInputSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay Input System",
		meta = (GameplayTagFilter = GameplayInput, AutoCreateRefTerm = "InputTag", WorldContext = "WorldContextObject"))
	static void InjectGameplayInput(UObject* WorldContextObject, const FGameplayTag& InputTag,
	                                const EGameplayInputType InputType);
};
