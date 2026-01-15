// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputSystemEnums.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "GameplayInputCommand.generated.h"

// TODO:
UENUM(BlueprintType)
enum class EGameplayInputCommandCaptureMode : uint8
{
	None UMETA(DisplayName = "None (不捕获)"),
	// 捕获失败时则清空已有捕获命令
	FailClearExisting UMETA(DisplayName = "Fail Clear Existing (捕获失败则清空已有捕获命令)"),
	//
};

USTRUCT(BlueprintType)
struct GAMEPLAYINPUTSYSTEM_API FGameplayInputCommand
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(Categories = "GameplayInput.InputSource"))
	FGameplayTag InputSourceTag;

	UPROPERTY(EditAnywhere)
	EGameplayInputType InputType;

	FGameplayInputCommand();

	FGameplayInputCommand(const FGameplayTag InputSourceTag, const EGameplayInputType InputType);

	// GetTypeHash
	friend uint32 GetTypeHash(const FGameplayInputCommand& Entry)
	{
		return HashCombine(GetTypeHash(Entry.InputSourceTag), GetTypeHash(static_cast<uint8>(Entry.InputType)));
	}

	bool operator==(const FGameplayInputCommand& Other) const;
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
class GAMEPLAYINPUTSYSTEM_API UGameplayInputCommandInstance : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(const FGameplayTag& InInputSourceTag, const EGameplayInputType InInputType, uint8 InPriority,
	                float InLifetime, float InTimestamp);

	void Initialize(const FGameplayTag& InInputSourceTag, const EGameplayInputType InInputType,
	                const FGameplayInputCommandConfig& InConfig);

public:
	FGameplayTag InputSourceTag;
	EGameplayInputType InputType;
	uint8 Priority = 0;
	float Lifetime = 0.0f;

	float Timestamp = 0.0f;
};
