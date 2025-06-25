// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputSubsystem.h"
#include "GameplayInteractionInstance.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "GameplayInteractionWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGameplayInteractionWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEPLAYINTERACTIONSYSTEM_API IGameplayInteractionWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Interaction|Lifecycle")
	void OnInitializeInteraction();

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Interaction|Lifecycle")
	void Cleanup();

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Interaction|Callbacks")
	void OnInteractionInputEvent(const FGameplayTag& InputTag, const EGameplayInputType InputType);

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Interaction|Callbacks")
	void OnInteractionSuccess();

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Interaction|Callbacks")
	void OnInteractionFailed();

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Interaction|Callbacks")
	void OnInteractionComplete();
};
