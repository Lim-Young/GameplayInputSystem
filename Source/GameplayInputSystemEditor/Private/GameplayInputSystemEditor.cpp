#include "GameplayInputSystemEditor.h"

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

void FGameplayInputSystemEditorModule::StartupModule()
{
	GameplayInputScenarioAssetTypeActions = MakeShared<FGameplayInputDocketAssetTypeActions>();

	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(
		GameplayInputScenarioAssetTypeActions.ToSharedRef());
}

void FGameplayInputSystemEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(
			GameplayInputScenarioAssetTypeActions.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameplayInputSystemEditorModule, GameplayInputSystemEditor)
