#include "GameplayInputSystemEditor.h"

#define LOCTEXT_NAMESPACE "FGameplayInputSystemEditorModule"

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
