#include "GameplayInputSystemEditor.h"

#include "GameplayInputArbiter.h"
#include "GameplayInputForwardingMapping.h"

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

UGameplayInputForwardingMappingFactory::UGameplayInputForwardingMappingFactory()
{
	SupportedClass = UGameplayInputForwardingMapping::StaticClass();
	bCreateNew = true;
}

UObject* UGameplayInputForwardingMappingFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
                                                                  EObjectFlags Flags, UObject* Context,
                                                                  FFeedbackContext* Warn)
{
	return NewObject<UGameplayInputForwardingMapping>(InParent, InClass, InName, Flags, Context);
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

UClass* FGameplayInputForwardingMappingAssetTypeActions::GetSupportedClass() const
{
	return UGameplayInputForwardingMapping::StaticClass();
}

FText FGameplayInputForwardingMappingAssetTypeActions::GetName() const
{
	return INVTEXT("Gameplay Input Forwarding Mapping");
}

FColor FGameplayInputForwardingMappingAssetTypeActions::GetTypeColor() const
{
	return FColor(149, 64, 180);
}

uint32 FGameplayInputForwardingMappingAssetTypeActions::GetCategories()
{
	return FAssetToolsModule::GetModule().Get().RegisterAdvancedAssetCategory(
		FName(TEXT("Input")), INVTEXT("Input"));
}

void FGameplayInputSystemEditorModule::StartupModule()
{
	GameplayInputScenarioAssetTypeActions = MakeShared<FGameplayInputDocketAssetTypeActions>();
	GameplayInputForwardingMappingAssetTypeActions = MakeShared<FGameplayInputForwardingMappingAssetTypeActions>();

	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(
		GameplayInputScenarioAssetTypeActions.ToSharedRef());
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(
		GameplayInputForwardingMappingAssetTypeActions.ToSharedRef());
}

void FGameplayInputSystemEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(
			GameplayInputScenarioAssetTypeActions.ToSharedRef());
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(
			GameplayInputForwardingMappingAssetTypeActions.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameplayInputSystemEditorModule, GameplayInputSystemEditor)
