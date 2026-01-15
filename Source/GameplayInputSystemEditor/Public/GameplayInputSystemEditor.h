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
class UGameplayInputDocketFactory : public UFactory
{
	GENERATED_BODY()

public:
	UGameplayInputDocketFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn) override;
};

UCLASS()
class UGameplayInputActionSetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UGameplayInputActionSetFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn) override;
};

// Asset Type Actions

/**
 * 
 */
class FGameplayInputDocketAssetTypeActions : public FAssetTypeActions_Base
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
class FGameplayInputActionSetAssetTypeActions : public FAssetTypeActions_Base
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
	TSharedPtr<FGameplayInputDocketAssetTypeActions> GameplayInputDocketAssetTypeActions;
	TSharedPtr<FGameplayInputActionSetAssetTypeActions> GameplayInputActionSetAssetTypeActions;
};
