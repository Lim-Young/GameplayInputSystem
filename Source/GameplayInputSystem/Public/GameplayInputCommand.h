// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputSystemEnums.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "GameplayInputCommand.generated.h"

USTRUCT(BlueprintType)
struct GAMEPLAYINPUTSYSTEM_API FGameplayInputCommandType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(Categories = GameplayInput))
	FGameplayTag InputTag;

	UPROPERTY(EditAnywhere)
	EGameplayInputType InputType;

	FGameplayInputCommandType();

	FGameplayInputCommandType(const FGameplayTag InputTag, const EGameplayInputType InputType);

	// GetTypeHash
	friend uint32 GetTypeHash(const FGameplayInputCommandType& Entry)
	{
		return HashCombine(GetTypeHash(Entry.InputTag), GetTypeHash(static_cast<uint8>(Entry.InputType)));
	}

	bool operator==(const FGameplayInputCommandType& Other) const;
};

USTRUCT(BlueprintType)
struct GAMEPLAYINPUTSYSTEM_API FGameplayInputCommandConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Input System")
	uint8 Priority = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Input System")
	float Lifetime = 0.0f;
};

UCLASS()
class GAMEPLAYINPUTSYSTEM_API UGameplayInputCommand : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(const FGameplayTag& InInputTag, const EGameplayInputType InInputType, uint8 InPriority,
	                float InLifetime, float InTimestamp);

	void Initialize(const FGameplayTag& InInputTag, const EGameplayInputType InInputType,
	                const FGameplayInputCommandConfig& InConfig);

public:
	FGameplayTag InputTag;
	EGameplayInputType InputType;
	uint8 Priority = 0;
	float Lifetime = 0.0f;

	float Timestamp = 0.0f;
};
