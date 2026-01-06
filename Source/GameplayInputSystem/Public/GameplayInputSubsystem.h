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
	Released,
	Action
};

UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayInput)

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
	                float InLifetime, float InTimestamp)
	{
		InputTag = InInputTag;
		InputType = InInputType;
		Priority = InPriority;
		Lifetime = InLifetime;

		Timestamp = InTimestamp;
	}

	void Initialize(const FGameplayTag& InInputTag, const EGameplayInputType InInputType,
	                const FGameplayInputCommandConfig& InConfig)
	{
		Initialize(InInputTag, InInputType, InConfig.Priority, InConfig.Lifetime, GetWorld()->GetTimeSeconds());
	}

public:
	FGameplayTag InputTag;
	EGameplayInputType InputType;
	uint8 Priority = 0;
	float Lifetime = 0.0f;

	float Timestamp = 0.0f;
};

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

/**
 * 
 */
UCLASS()
class GAMEPLAYINPUTSYSTEM_API UGameplayInputSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override
	{
		return Outer->GetWorld()->GetNetMode() != NM_DedicatedServer;
	}

private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameplayInputEvent, const FGameplayTag&, InputTag,
	                                             const EGameplayInputType, InputType);

public:
	UPROPERTY(BlueprintAssignable, Category = "Gameplay Input System|Events")
	FOnGameplayInputEvent OnGameplayInputEvent;

private:
	UPROPERTY()
	TMap<TObjectPtr<UGameplayInputDocket>, TObjectPtr<UGameplayInputArbiter>> GameplayInputArbiters;

public:
	void InjectGameplayInput(const FGameplayTag& InputTag, const EGameplayInputType InputType);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Input System")
	void CreateAndRegisterGameplayInputArbiter(UGameplayInputDocket* InGameplayInputDocker);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Input System")
	void FinishAndUnregisterGameplayInputArbiter(UGameplayInputDocket* InGameplayInputDocker);
};
