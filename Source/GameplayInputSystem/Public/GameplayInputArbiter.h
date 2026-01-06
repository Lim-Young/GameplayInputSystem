// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputStructsEnums.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GameplayInputArbiter.generated.h"

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
class UGameplayInputCommand : public UObject
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

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEPLAYINPUTSYSTEM_API UGameplayInputDocket : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Input System", meta = (Categories = GameplayInput))
	TMap<FGameplayTag, FGameplayInputCommandConfig> InputCommands;

	bool HasCommandForTag(const FGameplayTag& InputTag) const;

	FGameplayInputCommandConfig& GetCommandConfigForTag(const FGameplayTag& InputTag);
};

/**
 * 
 */
UCLASS()
class UGameplayInputArbiter : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UGameplayInputDocket* GameplayInputDocker;

	UPROPERTY()
	TArray<TObjectPtr<UGameplayInputCommand>> GameplayInputCommandQueue;

public:
	void Initialize(UGameplayInputDocket* InGameplayInputDocker);

	void Start();
	void Cancel();
	bool Finish(UGameplayInputCommand*& ResultCommand);

	bool ReceiveGameplayInput(FGameplayTag InputTag, EGameplayInputType InputType);
};
