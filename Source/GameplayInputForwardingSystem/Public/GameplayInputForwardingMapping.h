// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputSystemEnums.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GameplayInputForwardingMapping.generated.h"

// Input Source Command
USTRUCT(BlueprintType)
struct FGameplayInputSourceForwardingConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding")
	TMap<EGameplayInputType, FGameplayTag> InputTypeToForwardingTagMap;

	bool TryGetForwardingTag(const EGameplayInputType InInputType, FGameplayTag& OutForwardingTag) const;
};

USTRUCT(BlueprintType)
struct FGameplayInputSourceForwardingConfigContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding", meta = (Categories = "GameplayInput.InputSource"))
	FGameplayTag InputSourceTag;

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding")
	FGameplayInputSourceForwardingConfig ForwardingConfig;
};

// Input Action Event
USTRUCT(BlueprintType)
struct FGameplayInputActionForwardingConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding")
	TMap<EGameplayInputActionState, FGameplayTag> ActionStateToForwardingTagMap;

	bool TryGetForwardingTag(const EGameplayInputActionState InActionState, FGameplayTag& OutForwardingTag) const;
};

USTRUCT(BlueprintType)
struct FGameplayInputActionForwardingConfigContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding", meta = (Categories = "GameplayInput.InputAction"))
	FGameplayTag InputActionTag;

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding")
	FGameplayInputActionForwardingConfig ForwardingConfig;
};

/**
 * 
 */
UCLASS(AutoExpandCategories=("Gameplay Input Forwarding"))
class GAMEPLAYINPUTFORWARDINGSYSTEM_API UGameplayInputForwardingMapping : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding|Input Source",
		meta = (TitleProperty = "{InputSourceTag}"))
	TArray<FGameplayInputSourceForwardingConfigContainer> InputSourceForwardingConfigs;

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding|Input Action",
		meta = (TitleProperty = "{InputActionTag}"))
	TArray<FGameplayInputActionForwardingConfigContainer> InputActionForwardingConfigs;

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding")
	uint8 Priority = 0;

	UPROPERTY(VisibleAnywhere, Category = "Debug")
	TMap<FGameplayTag, FGameplayInputSourceForwardingConfig> InputSourceForwardingMap;

	UPROPERTY(VisibleAnywhere, Category = "Debug")
	TMap<FGameplayTag, FGameplayInputActionForwardingConfig> InputActionForwardingMap;

public:
	bool TryGetInputSourceForwardingTag(const FGameplayTag& InInputTag, const EGameplayInputType InInputType,
	                                    FGameplayTag& OutForwardingTag) const;

	bool TryGetInputActionForwardingTag(const FGameplayTag& InInputTag, const EGameplayInputActionState InActionState,
	                                    FGameplayTag& OutForwardingTag) const;

	uint8 GetPriority() const;

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
};

struct FGameplayInputForwardingMappingPredicate
{
	bool operator()(UGameplayInputForwardingMapping& A,
	                UGameplayInputForwardingMapping& B) const
	{
		return A.GetPriority() > B.GetPriority();
	}
};
