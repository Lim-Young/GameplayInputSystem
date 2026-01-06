// Copyright Lim Young.


#include "GameplayInputDocketAssetTypeActions.h"

#include "GameplayInputDocket.h"

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
