// Copyright Lim Young.


#include "GameplayInputForwardingMapping.h"

bool UGameplayInputForwardingMapping::TryGetInputForwardingTag(const FGameplayTag& InInputTag,
                                                               const EGameplayInputType InInputType,
                                                               FGameplayTag& OutForwardingTag) const
{
	const FGameplayInputCommandType Entry = FGameplayInputCommandType(InInputTag, InInputType);
	if (ForwardingMap.Contains(Entry))
	{
		OutForwardingTag = ForwardingMap[Entry];
		return true;
	}

	return false;
}

uint8 UGameplayInputForwardingMapping::GetPriority() const
{
	return Priority;
}
