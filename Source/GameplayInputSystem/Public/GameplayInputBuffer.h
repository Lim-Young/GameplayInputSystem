// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputActionEvent.h"
#include "GameplayInputSourceCommand.h"
#include "GameplayInputSystemEnums.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GameplayInputBuffer.generated.h"

UENUM()
enum class EGameplayInputBufferPolicy : uint8
{
	PriorityBased UMETA(DisplayName = "Priority Based (基于优先级)"),
	FirstSurvivalCommand UMETA(DisplayName = "First Survival Command (首个存活命令)"),
	LastSurvivalCommand UMETA(DisplayName = "Last Survival Command (末尾存活命令)"),
	FirstCommand UMETA(DisplayName = "First Command (首个命令)"),
	LastCommand UMETA(DisplayName = "Last Command (末尾命令)"),
};

UENUM(BlueprintType)
enum class EGameplayInputBufferScope : uint8
{
	InputSourceOnly UMETA(DisplayName = "Input Source Only"),
	InputActionOnly UMETA(DisplayName = "Input Action Only"),
	Both UMETA(DisplayName = "Both"),
};

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEPLAYINPUTSYSTEM_API UGameplayInputBufferSchema : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayInput")
	TMap<FGameplayInputSourceCommand, FGameplayInputSourceCommandConfig> InputSources;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayInput")
	TMap<FGameplayInputActionEvent, FGameplayInputActionEventConfig> InputActions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayInput")
	EGameplayInputBufferPolicy InputBufferPolicy = EGameplayInputBufferPolicy::PriorityBased;

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
class UGameplayInputBuffer : public UObject
{
	GENERATED_BODY()

	EGameplayInputBufferScope BufferScope;

protected:
	UPROPERTY()
	UGameplayInputBufferSchema* GameplayInputBufferSchema;

	UPROPERTY()
	TArray<TObjectPtr<UGameplayInputSourceCommandInstance>> GameplayInputSourceCommandQueue;

	UPROPERTY()
	TArray<TObjectPtr<UGameplayInputActionEventInstance>> GameplayInputActionEventQueue;

private:
	bool CheckIfTheCommandHasExpired(float CurrentTime, TObjectPtr<UGameplayInputSourceCommandInstance> CurrentCommand);
	bool CheckIfTheEventHasExpired(float CurrentTime, TObjectPtr<UGameplayInputActionEventInstance> CurrentEvent);
	bool BuffInputSourceCommand(UGameplayInputSourceCommandInstance*& ResultCommand, float CurrentTime);
	bool BuffInputActionEvent(UGameplayInputActionEventInstance*& ResultEvent, float CurrentTime);

public:
	void Initialize(UGameplayInputBufferSchema* InGameplayInputDocker, EGameplayInputBufferScope InBufferScope);

	void Start();
	void Cancel();
	bool Finish(UGameplayInputSourceCommandInstance*& ResultInputSourceCommand,
	            UGameplayInputActionEventInstance*& ResultInputActionEvent);

	bool ReceiveGameplayInputSourceCommand(FGameplayTag InputSourceTag, EGameplayInputType InputType);
	bool ReceiveGameplayInputActionEvent(const FGameplayTag& InputActionTag, EGameplayInputActionState ActionState);

	EGameplayInputBufferScope GetInputBufferScope() const;
	bool ShouldBuffInputSourceCommand() const;
	bool ShouldBuffInputActionEvent() const;
};
