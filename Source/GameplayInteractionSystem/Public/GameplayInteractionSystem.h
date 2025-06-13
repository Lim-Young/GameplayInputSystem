#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FGameplayInteractionSystemModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
