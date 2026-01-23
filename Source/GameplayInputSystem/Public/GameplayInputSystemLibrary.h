// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputSubsystem.h"
#include "GameplayInputSystemStructs.h"
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
		meta = (GameplayTagFilter = "GameplayInput.InputSource", AutoCreateRefTerm = "InputSourceTag", WorldContext =
			"WorldContextObject"))
	static void InjectGameplayInput(UObject* WorldContextObject, const FGameplayTag& InputSourceTag,
	                                const EGameplayInputType InputType);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Input System")
	static bool InputActionEventIsMatch(const FGameplayInputActionEventHandle& ActionEventHandle,
	                                    const FGameplayTag& InInputActionTag,
	                                    const EGameplayInputActionEvent InInputActionEvent);
};
