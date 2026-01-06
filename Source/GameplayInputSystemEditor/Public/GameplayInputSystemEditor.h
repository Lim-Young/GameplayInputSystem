#pragma once

#include "CoreMinimal.h"
#include "GameplayInputDocketAssetTypeActions.h"
#include "Modules/ModuleManager.h"

class FGameplayInputSystemEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
private:
    TSharedPtr<FGameplayInputDocketAssetTypeActions> GameplayInputScenarioAssetTypeActions;
};
