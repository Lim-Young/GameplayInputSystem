// Copyright Lim Young.


#include "FrameCountCustomization.h"

#include "DetailWidgetRow.h"
#include "Editor/EditorWidgets/Public/SEnumCombo.h"
#include "Widgets/Input/SNumericEntryBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FrameCountCustomization)

#define LOCTEXT_NAMESPACE "FrameCountCustomization"

namespace GameplayInputSystemEditor
{
	static float SecondsToFrames = 60.0f;
	static float FramesToSeconds = 1.0f / 60.0f;
	static EFrameCountFloatFormat GTimeFloatDisplayFormat = EFrameCountFloatFormat::Frames;

	void FFrameCountCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
	                                               FDetailWidgetRow& HeaderRow,
	                                               IPropertyTypeCustomizationUtils& CustomizationUtils)
	{
		if (PropertyHandle->HasMetaData(TEXT("FrameRate")))
		{
			SecondsToFrames = PropertyHandle->GetIntMetaData(TEXT("FrameRate"));
			FramesToSeconds = 1.0f / SecondsToFrames;
		}

		UEnum* TimeFloatFormatEnum = StaticEnum<EFrameCountFloatFormat>();
		
		HeaderRow
			.NameContent()
			[
				PropertyHandle->CreatePropertyNameWidget()
			]
			.ValueContent()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					
					SNew(SBox).WidthOverride(125).HeightOverride(20)
					[
						SNew(SNumericEntryBox<float>)
						.MinFractionalDigits(0)
						.MaxFractionalDigits(3)
						.Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
						.OverrideTextMargin(FMargin(8, 0, 8, 0))
						.AllowSpin(true)
						.Value_Lambda([PropertyHandle]()
						{
							float Value;
							PropertyHandle->GetValue(Value);
							return GTimeFloatDisplayFormat == EFrameCountFloatFormat::Frames
								       ? Value * SecondsToFrames
								       : Value;
						})
						// 拖动时实时更新数值
						.OnValueChanged_Lambda([PropertyHandle](float NewValue)
						{
							PropertyHandle->NotifyPreChange();
							PropertyHandle->EnumerateRawData([NewValue](void* RawData, int32 Index, int32 Num)
							{
								*static_cast<float*>(RawData) = GTimeFloatDisplayFormat == EFrameCountFloatFormat::Frames
								                                ? NewValue * FramesToSeconds
								                                : NewValue;
								return true;
							});
							PropertyHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
						})
						// 输入完成后更新数值
						.OnValueCommitted_Lambda([PropertyHandle](float NewValue, ETextCommit::Type CommitType)
						{
							FScopedTransaction ScopedTransaction(
								LOCTEXT("Set Frame Time Value", "Set Frame Time Value"));
							PropertyHandle->NotifyPreChange();
							PropertyHandle->EnumerateRawData([NewValue](void* RawData, int32 Index, int32 Num)
							{
								*static_cast<float*>(RawData) = GTimeFloatDisplayFormat ==
								                                EFrameCountFloatFormat::Frames
									                                ? NewValue * FramesToSeconds
									                                : NewValue;
								return true;
							});
							PropertyHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
						})
					]
				]
				+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(SBox).WidthOverride(80).HeightOverride(20)
					[
						SNew(SEnumComboBox, TimeFloatFormatEnum)
						.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
						.CurrentValue_Lambda([] { return static_cast<int32>(GTimeFloatDisplayFormat); })
						.ContentPadding(FMargin(0.0f, 0.0f))
						.ToolTipText(LOCTEXT("FrameTimeToolTip",
						                     "Select whether to display and edit the time as Seconds, or Frames."))
						.OnEnumSelectionChanged_Lambda([](uint32 NewValue, ESelectInfo::Type)
						{
							FSlateApplication::Get().ClearKeyboardFocus(EFocusCause::Cleared);
							GTimeFloatDisplayFormat = static_cast<EFrameCountFloatFormat>(NewValue);
						})
					]
				]
				+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(FMargin(10, 0, 0, 0))
				[
					SNew(STextBlock)
					.Font(FAppStyle::GetFontStyle("PropertyWindow.ItalicFont"))
					.Text_Lambda([PropertyHandle]()
					{
						float Value;
						PropertyHandle->GetValue(Value);
						const FString DisplayString = GTimeFloatDisplayFormat == EFrameCountFloatFormat::Frames
							                              ? FString::Printf(
								                              TEXT("%.3f s / %.0f fps"), Value, SecondsToFrames)
							                              : FString::Printf(
								                              TEXT("%.0f frames / %.0f fps"),
								                              Value * SecondsToFrames, SecondsToFrames);
						return FText::FromString(DisplayString);
					})
				]
			];
	}


	void FFrameCountCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	                                                 IDetailChildrenBuilder& ChildBuilder,
	                                                 IPropertyTypeCustomizationUtils& CustomizationUtils)
	{
	}
}

#undef LOCTEXT_NAMESPACE
