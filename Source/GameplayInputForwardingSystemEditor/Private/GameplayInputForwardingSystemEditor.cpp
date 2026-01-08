#include "GameplayInputForwardingSystemEditor.h"

#include "GameplayInputForwardingMapping.h"

#define LOCTEXT_NAMESPACE "FGameplayInputForwardingSystemEditorModule"

// Factory

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


void FGameplayInputForwardingSystemEditorModule::StartupModule()
{
	GameplayInputForwardingMappingAssetTypeActions = MakeShared<FGameplayInputForwardingMappingAssetTypeActions>();

	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(
		GameplayInputForwardingMappingAssetTypeActions.ToSharedRef());
}

void FGameplayInputForwardingSystemEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(
			GameplayInputForwardingMappingAssetTypeActions.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameplayInputForwardingSystemEditorModule, GameplayInputForwardingSystemEditor)
