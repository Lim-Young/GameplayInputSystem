// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FrameCountCustomization.generated.h"

UENUM()
enum class EFrameCountFloatFormat
{
	Frames,
	Seconds,
};

namespace GameplayInputSystemEditor
{
	class FFrameCountPropertyTypeIdentifier : public IPropertyTypeIdentifier
	{
	public:
		virtual bool IsPropertyTypeCustomized(const IPropertyHandle& PropertyHandle) const override
		{
			return PropertyHandle.HasMetaData(TEXT("EditAsFrames"));
		}
	};

	class FFrameCountCustomization : public IPropertyTypeCustomization
	{
	public:
		// IPropertyTypeCustomization interface
		virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
		                             IPropertyTypeCustomizationUtils& CustomizationUtils) override;
		virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
		                               IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	};
}
