// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayInputSubsystem.generated.h"

UENUM(BlueprintType)
enum class EGameplayInputType : uint8
{
	Pressed,
	Released
};

UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayInput)

/**
 * 
 */
UCLASS()
class GAMEPLAYINPUTSYSTEM_API UGameplayInputSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameplayInputEvent, FGameplayTag, InputTag, EGameplayInputType,
	                                             InputType);

public:
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Input System|Events")
	FOnGameplayInputEvent OnGameplayInputEvent;

public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay Input System", meta = (GameplayTagFilter = GameplayInput))
	void InjectGameplayInput(const FGameplayTag& InputTag, const EGameplayInputType InputType) const;
};
