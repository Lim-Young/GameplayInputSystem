// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputSourceCommand.h"
#include "GameplayInputSystemEnums.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GameplayInputForwardingMapping.generated.h"

USTRUCT(BlueprintType)
struct FGameplayInputForwardingConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding")
	TMap<EGameplayInputType, FGameplayTag> InputTypeToForwardingTagMap;

	bool TryGetForwardingTag(const EGameplayInputType InInputType, FGameplayTag& OutForwardingTag) const;
};

USTRUCT(BlueprintType)
struct FGameplayInputForwardingConfigContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding", meta = (Categories = GameplayInput))
	FGameplayTag InputTag;

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding")
	FGameplayInputForwardingConfig ForwardingConfig;
};

/**
 * 
 */
UCLASS(AutoExpandCategories=("Gameplay Input Forwarding"))
class GAMEPLAYINPUTFORWARDINGSYSTEM_API UGameplayInputForwardingMapping : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding",
		meta = (TitleProperty = "{InputTag}"))
	TArray<FGameplayInputForwardingConfigContainer> ForwardingConfigs;

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding")
	uint8 Priority = 0;

	UPROPERTY(VisibleAnywhere, Category = "Debug")
	TMap<FGameplayTag, FGameplayInputForwardingConfig> ForwardingMap;

public:
	bool TryGetInputForwardingTag(const FGameplayTag& InInputTag, const EGameplayInputType InInputType,
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
