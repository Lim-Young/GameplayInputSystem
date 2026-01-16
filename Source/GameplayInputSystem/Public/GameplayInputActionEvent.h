// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputSystemEnums.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GameplayInputActionEvent.generated.h"


USTRUCT(BlueprintType)
struct GAMEPLAYINPUTSYSTEM_API FGameplayInputActionEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(Categories = "GameplayInput.InputAction"))
	FGameplayTag InputActionTag;

	EGameplayInputActionState InputActionState;

	FGameplayInputActionEvent();

	FGameplayInputActionEvent(const FGameplayTag InputSourceTag);

	FGameplayInputActionEvent(const FGameplayTag InputSourceTag, EGameplayInputActionState InInputActionState);

	// GetTypeHash
	friend uint32 GetTypeHash(const FGameplayInputActionEvent& Entry)
	{
		return HashCombine(GetTypeHash(Entry.InputActionTag), GetTypeHash(Entry.InputActionState));
	}

	bool operator==(const FGameplayInputActionEvent& Other) const;
};

USTRUCT(BlueprintType)
struct GAMEPLAYINPUTSYSTEM_API FGameplayInputActionEventConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = "/Script/GameplayInputSystem.EGameplayInputActionState"))
	int32 ListenedActionStates = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Input System")
	uint8 Priority = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Input System")
	float Lifetime = 0.0f;
};

UCLASS()
class GAMEPLAYINPUTSYSTEM_API UGameplayInputActionEventInstance : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(const FGameplayTag& InInputActionTag, const EGameplayInputActionState InInputActionState,
	                uint8 InPriority,
	                float InLifetime, float InTimestamp);

	void Initialize(const FGameplayTag& InInputActionTag, const EGameplayInputActionState InInputActionState,
	                const FGameplayInputActionEventConfig& InConfig);

public:
	FGameplayTag InputActionTag;
	EGameplayInputActionState InputActionState;
	uint8 Priority = 0;
	float Lifetime = 0.0f;

	float Timestamp = 0.0f;
};
