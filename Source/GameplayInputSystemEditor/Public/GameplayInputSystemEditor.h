#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "Modules/ModuleManager.h"

#include "GameplayInputSystemEditor.generated.h"

// Factory

/**
 * 
 */
UCLASS()
class GAMEPLAYINPUTSYSTEMEDITOR_API UGameplayInputDocketFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UGameplayInputDocketFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
		UObject* Context, FFeedbackContext* Warn) override;
};

UCLASS()
class GAMEPLAYINPUTSYSTEMEDITOR_API UGameplayInputForwardingMappingFactory : public UFactory
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
class GAMEPLAYINPUTSYSTEMEDITOR_API FGameplayInputDocketAssetTypeActions : public FAssetTypeActions_Base
{
public:
	virtual UClass* GetSupportedClass() const override;
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
};

/**
 * 
 */
class GAMEPLAYINPUTSYSTEMEDITOR_API FGameplayInputForwardingMappingAssetTypeActions : public FAssetTypeActions_Base
{
public:
	virtual UClass* GetSupportedClass() const override;
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
};

class FGameplayInputSystemEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FGameplayInputDocketAssetTypeActions> GameplayInputScenarioAssetTypeActions;
	TSharedPtr<FGameplayInputForwardingMappingAssetTypeActions> GameplayInputForwardingMappingAssetTypeActions;
};
