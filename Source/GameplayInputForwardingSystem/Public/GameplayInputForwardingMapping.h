// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputCommand.h"
#include "GameplayInputSystemEnums.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GameplayInputForwardingMapping.generated.h"

/**
 * 
 */
UCLASS(AutoExpandCategories=("Gameplay Input Forwarding"))
class GAMEPLAYINPUTFORWARDINGSYSTEM_API UGameplayInputForwardingMapping : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Gameplay Input Forwarding", meta = (TitleProperty = "{MyString}[{MyInt}]"))
	TMap<FGameplayInputCommandType, FGameplayTag> ForwardingMap;

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
