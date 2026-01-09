// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputSystemEnums.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "GameplayInputCommand.generated.h"

USTRUCT(BlueprintType)
struct FGameplayInputCommandConfig
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
