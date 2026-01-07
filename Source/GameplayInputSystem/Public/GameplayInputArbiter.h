// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputSystemEnums.h"
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

UENUM()
enum class EArbiterDeliberationMode : uint8
{
	PriorityBased UMETA(DisplayName = "Priority Based (基于优先级)"),
	FirstSurvivalCommand UMETA(DisplayName = "First Survival Command (首个存活命令)"),
	LastSurvivalCommand UMETA(DisplayName = "Last Survival Command (末尾存活命令)"),
	FirstCommand UMETA(DisplayName = "First Command (首个命令)"),
	LastCommand UMETA(DisplayName = "Last Command (末尾命令)"),
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Input System")
	EArbiterDeliberationMode DeliberationMode = EArbiterDeliberationMode::PriorityBased;

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

private:
	bool CheckIfTheCommandHasExpired(float CurrentTime, TObjectPtr<UGameplayInputCommand> CurrentCommand);

public:
	void Initialize(UGameplayInputDocket* InGameplayInputDocker);

	void Start();
	void Cancel();
	bool Finish(UGameplayInputCommand*& ResultCommand);

	bool ReceiveGameplayInput(FGameplayTag InputTag, EGameplayInputType InputType);
};
