// Copyright Lim Young.


#include "GameplayInputForwardingMapping.h"

bool FGameplayInputForwardingConfig::TryGetForwardingTag(const EGameplayInputType InInputType,
                                                         FGameplayTag& OutForwardingTag) const
{
	if (InputTypeToForwardingTagMap.Contains(InInputType))
	{
		OutForwardingTag = InputTypeToForwardingTagMap[InInputType];
		return true;
	}

	return false;
}

bool UGameplayInputForwardingMapping::TryGetInputForwardingTag(const FGameplayTag& InInputTag,
                                                               const EGameplayInputType InInputType,
                                                               FGameplayTag& OutForwardingTag) const
{
	if (ForwardingMap.Contains(InInputTag))
	{
		return ForwardingMap[InInputTag].TryGetForwardingTag(InInputType, OutForwardingTag);
	}

	return false;
}

uint8 UGameplayInputForwardingMapping::GetPriority() const
{
	return Priority;
}

#if WITH_EDITOR
void UGameplayInputForwardingMapping::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd)
	{
		const int32 Index = PropertyChangedEvent.GetArrayIndex(
			GET_MEMBER_NAME_CHECKED(UGameplayInputForwardingMapping, ForwardingConfigs).ToString());

		// if array already has empty tag, do not add new one
		for (int i = 0; i < ForwardingConfigs.Num(); i++)
		{
			if (i != Index && !ForwardingConfigs[i].InputTag.IsValid())
			{
				ForwardingConfigs.RemoveAt(Index);
				FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(
					                     "GameplayInputForwardingMapping: Cannot add new entry when there is an empty InputTag."));
				return;
			}
		}
	}

	// Check ForwardingConfigs InputTag uniqueness
	TSet<FGameplayTag> UniqueTags;
	// if new tag already exists, clear it and show warning
	for (FGameplayInputForwardingConfigContainer& ConfigContainer : ForwardingConfigs)
	{
		if (!ConfigContainer.InputTag.IsValid())
		{
			continue;
		}

		if (UniqueTags.Contains(ConfigContainer.InputTag))
		{
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(
				                     "GameplayInputForwardingMapping: Duplicate InputTags are not allowed in ForwardingConfigs."));
			ConfigContainer.InputTag = FGameplayTag();
		}
		else
		{
			UniqueTags.Add(ConfigContainer.InputTag);
		}
	}

	// Rebuild ForwardingMap
	ForwardingMap.Empty();
	for (const FGameplayInputForwardingConfigContainer& ConfigContainer : ForwardingConfigs)
	{
		if (!ConfigContainer.InputTag.IsValid())
		{
			continue;
		}

		ForwardingMap.Add(ConfigContainer.InputTag, ConfigContainer.ForwardingConfig);
	}
}
#endif
