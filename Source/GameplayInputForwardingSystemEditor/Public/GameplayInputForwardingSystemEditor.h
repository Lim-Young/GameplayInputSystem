#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "AssetTypeActions_Base.h"
#include "GameplayInputForwardingSystemEditor.generated.h"

// Factory

UCLASS()
class UGameplayInputForwardingMappingFactory : public UFactory
{
	GENERATED_BODY()

public:
	UGameplayInputForwardingMappingFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn) override;
};

// Asset Type Actions

/**
 * 
 */
class FGameplayInputForwardingMappingAssetTypeActions : public FAssetTypeActions_Base
{
public:
	virtual UClass* GetSupportedClass() const override;
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
};

class FGameplayInputForwardingSystemEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FGameplayInputForwardingMappingAssetTypeActions> GameplayInputForwardingMappingAssetTypeActions;
};
