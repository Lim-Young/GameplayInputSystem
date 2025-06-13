// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInteractionInstance.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayInteractionSystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYINTERACTIONSYSTEM_API UGameplayInteractionSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Gameplay Interaction System", meta = (WorldContext = "WorldContextObject"))
	static UGameplayInteractionInstance* CreateGameplayInteractionInstance(UObject* WorldContextObject,UPARAM(ref)
	                                                                       FGameplayInteractionDescription&
	                                                                       Description);
};
