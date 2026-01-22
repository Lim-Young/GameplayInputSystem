// Copyright Lim Young.


#include "GameplayInputForwardingMapping.h"

bool FGameplayInputSourceForwardingConfig::TryGetForwardingTag(const EGameplayInputType InInputType,
                                                               FGameplayTag& OutForwardingTag) const
{
	if (InputTypeToForwardingTagMap.Contains(InInputType))
	{
		OutForwardingTag = InputTypeToForwardingTagMap[InInputType];
		return true;
	}

	return false;
}

bool FGameplayInputActionForwardingConfig::TryGetForwardingTag(const EGameplayInputActionEvent InActionState,
                                                               FGameplayTag& OutForwardingTag) const
{
	if (ActionStateToForwardingTagMap.Contains(InActionState))
	{
		OutForwardingTag = ActionStateToForwardingTagMap[InActionState];
		return true;
	}

	return false;
}

bool UGameplayInputForwardingMapping::TryGetInputSourceForwardingTag(const FGameplayTag& InInputTag,
                                                                     const EGameplayInputType InInputType,
                                                                     FGameplayTag& OutForwardingTag) const
{
	if (InputSourceForwardingMap.Contains(InInputTag))
	{
		return InputSourceForwardingMap[InInputTag].TryGetForwardingTag(InInputType, OutForwardingTag);
	}

	return false;
}

bool UGameplayInputForwardingMapping::TryGetInputActionForwardingTag(const FGameplayTag& InInputTag,
                                                                     const EGameplayInputActionEvent InActionState,
                                                                     FGameplayTag& OutForwardingTag) const
{
	if (InputActionForwardingMap.Contains(InInputTag))
	{
		return InputActionForwardingMap[InInputTag].TryGetForwardingTag(InActionState, OutForwardingTag);
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

	if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd ||
		PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate)
	{
		int32 Index;

		// If Input Source Array
		if (PropertyChangedEvent.Property &&
			PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(
				UGameplayInputForwardingMapping, InputSourceForwardingConfigs))
		{
			Index = PropertyChangedEvent.GetArrayIndex(
				GET_MEMBER_NAME_CHECKED(UGameplayInputForwardingMapping, InputSourceForwardingConfigs).ToString());

			// if array already has empty tag, do not add new one
			for (int i = 0; i < InputSourceForwardingConfigs.Num(); i++)
			{
				if (i != Index && !InputSourceForwardingConfigs[i].InputSourceTag.IsValid())
				{
					InputSourceForwardingConfigs.RemoveAt(Index);
					FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(
						                     "GameplayInputForwardingMapping: Cannot add new entry when there is an empty InputTag."));
					return;
				}
			}
		}
		else if (PropertyChangedEvent.Property &&
			PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(
				UGameplayInputForwardingMapping, InputActionForwardingConfigs))
		{
			Index = PropertyChangedEvent.GetArrayIndex(
				GET_MEMBER_NAME_CHECKED(UGameplayInputForwardingMapping, InputActionForwardingConfigs).ToString());

			// if array already has empty tag, do not add new one
			for (int i = 0; i < InputActionForwardingConfigs.Num(); i++)
			{
				if (i != Index && !InputActionForwardingConfigs[i].InputActionTag.IsValid())
				{
					InputActionForwardingConfigs.RemoveAt(Index);
					FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(
						                     "GameplayInputForwardingMapping: Cannot add new entry when there is an empty InputTag."));
					return;
				}
			}
		}
	}

	// Check ForwardingConfigs InputTag uniqueness
	if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ValueSet ||
		PropertyChangedEvent.ChangeType == EPropertyChangeType::ResetToDefault ||
		PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate)
	{
		if (PropertyChangedEvent.Property &&
			PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(
				FGameplayInputSourceForwardingConfigContainer, InputSourceTag))
		{
			int index = PropertyChangedEvent.GetArrayIndex(
				GET_MEMBER_NAME_CHECKED(UGameplayInputForwardingMapping, InputSourceForwardingConfigs).ToString());
			if (index >= 0)
			{
				FGameplayTag ChangedTag = InputSourceForwardingConfigs[index].InputSourceTag;
				for (int i = 0; i < InputSourceForwardingConfigs.Num(); i++)
				{
					if (i != index && InputSourceForwardingConfigs[i].InputSourceTag == ChangedTag)
					{
						FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(
							                     "GameplayInputForwardingMapping: Duplicate InputSourceTag is not allowed."));
						InputSourceForwardingConfigs[index].InputSourceTag = FGameplayTag();
						break;
					}
				}
			}
		}
		else if (PropertyChangedEvent.Property &&
			PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(
				FGameplayInputActionForwardingConfigContainer, InputActionTag))
		{
			int index = PropertyChangedEvent.GetArrayIndex(
				GET_MEMBER_NAME_CHECKED(UGameplayInputForwardingMapping, InputActionForwardingConfigs).ToString());
			if (index >= 0)
			{
				FGameplayTag ChangedTag = InputActionForwardingConfigs[index].InputActionTag;
				for (int i = 0; i < InputActionForwardingConfigs.Num(); i++)
				{
					if (i != index && InputActionForwardingConfigs[i].InputActionTag == ChangedTag)
					{
						FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(
							                     "GameplayInputForwardingMapping: Duplicate InputActionTag is not allowed."));
						InputActionForwardingConfigs[index].InputActionTag = FGameplayTag();
						break;
					}
				}
			}
		}
	}

	// Rebuild Maps
	InputSourceForwardingMap.Empty();
	for (const FGameplayInputSourceForwardingConfigContainer& ConfigContainer : InputSourceForwardingConfigs)
	{
		if (!ConfigContainer.InputSourceTag.IsValid())
		{
			continue;
		}

		InputSourceForwardingMap.Add(ConfigContainer.InputSourceTag, ConfigContainer.ForwardingConfig);
	}

	InputActionForwardingMap.Empty();
	for (const FGameplayInputActionForwardingConfigContainer& ConfigContainer : InputActionForwardingConfigs)
	{
		if (!ConfigContainer.InputActionTag.IsValid())
		{
			continue;
		}

		InputActionForwardingMap.Add(ConfigContainer.InputActionTag, ConfigContainer.ForwardingConfig);
	}
}
#endif
