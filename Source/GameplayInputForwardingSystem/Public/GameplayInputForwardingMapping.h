// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputSystemEnums.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GameplayInputForwardingMapping.generated.h"

USTRUCT(BlueprintType)
struct FGameplayInputForwardingEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(Categories = GameplayInput))
	FGameplayTag InputTag;

	UPROPERTY(EditAnywhere)
	EGameplayInputType InputType;

	FGameplayInputForwardingEntry()
	{
		InputTag = FGameplayTag::EmptyTag;
		InputType = EGameplayInputType::Action;
	}

	FGameplayInputForwardingEntry(const FGameplayTag InputTag, const EGameplayInputType InputType)
	{
		this->InputTag = InputTag;
		this->InputType = InputType;
	}

	// GetTypeHash
	friend uint32 GetTypeHash(const FGameplayInputForwardingEntry& Entry)
	{
		return HashCombine(GetTypeHash(Entry.InputTag), GetTypeHash(static_cast<uint8>(Entry.InputType)));
	}
	
	bool operator==(const FGameplayInputForwardingEntry& Other) const
	{
		return InputTag == Other.InputTag && InputType == Other.InputType;
	}
};

/**
 * 
 */
UCLASS(AutoExpandCategories=("Gameplay Input Forwarding"))
class GAMEPLAYINPUTFORWARDINGSYSTEM_API UGameplayInputForwardingMapping : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding", meta = (TitleProperty = "{MyString}[{MyInt}]"))
	TMap<FGameplayInputForwardingEntry, FGameplayTag> ForwardingMap;

	UPROPERTY(EditAnywhere)
	uint8 Priority = 0;

public:
	bool TryGetInputForwardingTag(const FGameplayTag& InInputTag, const EGameplayInputType InInputType,
	                              FGameplayTag& OutForwardingTag) const;

	uint8 GetPriority() const;
};

struct FGameplayInputForwardingMappingPredicate
{
	bool operator()(UGameplayInputForwardingMapping& A,
	                UGameplayInputForwardingMapping& B) const
	{
		return A.GetPriority() > B.GetPriority();
	}
};
