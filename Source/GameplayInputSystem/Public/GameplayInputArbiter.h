// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputSourceCommand.h"
#include "GameplayInputSystemEnums.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GameplayInputArbiter.generated.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Input System",
		meta = (TitleProperty = "{InputSourceTag}[{InputType}]"))
	TMap<FGameplayInputSourceCommand, FGameplayInputSourceCommandConfig> InputSources;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Input System")
	EArbiterDeliberationMode DeliberationMode = EArbiterDeliberationMode::PriorityBased;

	bool HasCommand(const FGameplayTag& InputSourceTag, EGameplayInputType InputType) const;

	FGameplayInputSourceCommandConfig& GetCommandConfig(const FGameplayTag& InputSourceTag, EGameplayInputType InputType);
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
	TArray<TObjectPtr<UGameplayInputSourceCommandInstance>> GameplayInputCommandQueue;

private:
	bool CheckIfTheCommandHasExpired(float CurrentTime, TObjectPtr<UGameplayInputSourceCommandInstance> CurrentCommand);

public:
	void Initialize(UGameplayInputDocket* InGameplayInputDocker);

	void Start();
	void Cancel();
	bool Finish(UGameplayInputSourceCommandInstance*& ResultCommand);

	bool ReceiveGameplayInput(FGameplayTag InputSourceTag, EGameplayInputType InputType);
};
