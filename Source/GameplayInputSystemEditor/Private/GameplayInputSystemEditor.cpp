#include "GameplayInputSystemEditor.h"

#include "FrameCountCustomization.h"
#include "InputAction/GameplayInputActionSet.h"
#include "GameplayInputArbiter.h"

#define LOCTEXT_NAMESPACE "FGameplayInputSystemEditorModule"

// Factory

UGameplayInputDocketFactory::UGameplayInputDocketFactory()
{
	SupportedClass = UGameplayInputDocket::StaticClass();
	bCreateNew = true;
}

UObject* UGameplayInputDocketFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
                                                       EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UGameplayInputDocket>(InParent, InClass, InName, Flags, Context);
}

UGameplayInputActionSetFactory::UGameplayInputActionSetFactory()
{
	SupportedClass = UGameplayInputActionSet::StaticClass();
	bCreateNew = true;
}

UObject* UGameplayInputActionSetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
                                                          EObjectFlags Flags, UObject* Context,
                                                          FFeedbackContext* Warn)
{
	return NewObject<UGameplayInputActionSet>(InParent, InClass, InName, Flags, Context);
}

// Asset Type Actions

UClass* FGameplayInputDocketAssetTypeActions::GetSupportedClass() const
{
	return UGameplayInputDocket::StaticClass();
}

FText FGameplayInputDocketAssetTypeActions::GetName() const
{
	return INVTEXT("Gameplay Input Docket");
}

FColor FGameplayInputDocketAssetTypeActions::GetTypeColor() const
{
	return FColor(26, 96, 160);
}

uint32 FGameplayInputDocketAssetTypeActions::GetCategories()
{
	return FAssetToolsModule::GetModule().Get().RegisterAdvancedAssetCategory(
		FName(TEXT("Input")), INVTEXT("Input"));
}

UClass* FGameplayInputActionSetAssetTypeActions::GetSupportedClass() const
{
	return UGameplayInputActionSet::StaticClass();
}

FText FGameplayInputActionSetAssetTypeActions::GetName() const
{
	return INVTEXT("Gameplay Input Action Set");
}

FColor FGameplayInputActionSetAssetTypeActions::GetTypeColor() const
{
	return FColor(26, 160, 96);
}

uint32 FGameplayInputActionSetAssetTypeActions::GetCategories()
{
	return FAssetToolsModule::GetModule().Get().RegisterAdvancedAssetCategory(
		FName(TEXT("Input")), INVTEXT("Input"));
}


void FGameplayInputSystemEditorModule::StartupModule()
{
	GameplayInputDocketAssetTypeActions = MakeShared<FGameplayInputDocketAssetTypeActions>();
	GameplayInputActionSetAssetTypeActions = MakeShared<FGameplayInputActionSetAssetTypeActions>();

	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(
		GameplayInputDocketAssetTypeActions.ToSharedRef());
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(
		GameplayInputActionSetAssetTypeActions.ToSharedRef());

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(
		"PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout("FloatProperty",
	                                                FOnGetPropertyTypeCustomizationInstance::CreateLambda([]
	                                                {
		                                                return MakeShared<
			                                                GameplayInputSystemEditor::FFrameCountCustomization>();
	                                                }), MakeShared<
		                                                GameplayInputSystemEditor::FFrameCountPropertyTypeIdentifier>());
}

void FGameplayInputSystemEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(
			GameplayInputDocketAssetTypeActions.ToSharedRef());
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(
			GameplayInputActionSetAssetTypeActions.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameplayInputSystemEditorModule, GameplayInputSystemEditor)
