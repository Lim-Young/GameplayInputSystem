// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputActionEvent.h"
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

UENUM()
enum class EArbiterCommandMatchMode : uint8
{
	InputSourceOnly UMETA(DisplayName = "Input Source Only"),
	InputActionOnly UMETA(DisplayName = "Input Action Only"),
	Both UMETA(DisplayName = "Both"),
};

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEPLAYINPUTSYSTEM_API UGameplayInputDocket : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayInput|InputSource",
		meta = (TitleProperty = "{InputSourceTag}[{InputType}]"))
	TMap<FGameplayInputSourceCommand, FGameplayInputSourceCommandConfig> InputSources;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayInput|InputAction")
	TMap<FGameplayInputActionEvent, FGameplayInputActionEventConfig> InputActions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayInput")
	EArbiterDeliberationMode DeliberationMode = EArbiterDeliberationMode::PriorityBased;

	bool HasInputSourceCommand(const FGameplayTag& InputSourceTag, EGameplayInputType InputType) const;
	bool HasInputActionEvent(const FGameplayTag& InputActionTag, EGameplayInputActionState ActionState) const;

	FGameplayInputSourceCommandConfig& GetInputSourceCommandConfig(const FGameplayTag& InputSourceTag,
	                                                               EGameplayInputType InputType);
	FGameplayInputActionEventConfig& GetInputActionEventConfig(const FGameplayTag& InputActionTag);
};

/**
 * 
 */
UCLASS()
class UGameplayInputArbiter : public UObject
{
	GENERATED_BODY()

	EArbiterCommandMatchMode MatchMode;

protected:
	UPROPERTY()
	UGameplayInputDocket* GameplayInputDocker;

	UPROPERTY()
	TArray<TObjectPtr<UGameplayInputSourceCommandInstance>> GameplayInputCommandQueue;

	UPROPERTY()
	TArray<TObjectPtr<UGameplayInputActionEventInstance>> GameplayInputActionEventQueue;

private:
	bool CheckIfTheCommandHasExpired(float CurrentTime, TObjectPtr<UGameplayInputSourceCommandInstance> CurrentCommand);
	bool CheckIfTheEventHasExpired(float CurrentTime, TObjectPtr<UGameplayInputActionEventInstance> CurrentEvent);
	bool MarchInputSourceCommand(UGameplayInputSourceCommandInstance*& ResultCommand, float CurrentTime);
	bool MarchInputActionEvent(UGameplayInputActionEventInstance*& ResultEvent, float CurrentTime);

public:
	void Initialize(UGameplayInputDocket* InGameplayInputDocker, EArbiterCommandMatchMode InMatchMode);

	void Start();
	void Cancel();
	bool Finish(UGameplayInputSourceCommandInstance*& ResultInputSourceCommand,
	            UGameplayInputActionEventInstance*& ResultInputActionEvent);

	bool ReceiveGameplayInputSourceCommand(FGameplayTag InputSourceTag, EGameplayInputType InputType);
	bool ReceiveGameplayInputActionEvent(const FGameplayTag& InputActionTag, EGameplayInputActionState ActionState);

	EArbiterCommandMatchMode GetMatchMode() const;
	bool ShouldMatchInputSourceCommand() const;
	bool ShouldMatchInputActionEvent() const;
};
