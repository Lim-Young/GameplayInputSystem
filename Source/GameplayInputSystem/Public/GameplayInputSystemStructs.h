// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "GameplayInputSystemEnums.h"
#include "GameplayInputSystemStructs.generated.h"

USTRUCT(BlueprintType)
struct GAMEPLAYINPUTSYSTEM_API FGameplayInputActionEventHandle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories = "GameplayInput.InputAction"))
	FGameplayTag InputActionTag;

	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = "/Script/GameplayInputSystem.EGameplayInputActionEvent"))
	int32 InputActionEvents = 0;

	bool IsMatch(const FGameplayTag& InInputActionTag, const EGameplayInputActionEvent InInputActionEvent) const;
};
