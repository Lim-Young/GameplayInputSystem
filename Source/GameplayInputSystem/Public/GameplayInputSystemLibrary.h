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
		meta = (GameplayTagFilter = GameplayInput_Source, AutoCreateRefTerm = "InputTag", WorldContext = "WorldContextObject"))
	static void InjectGameplayInput(UObject* WorldContextObject, const FGameplayTag& InputSourceTag,
	                                const EGameplayInputType InputType);
};
