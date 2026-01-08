// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayInputForwardingSystemSettings.generated.h"

/**
 * 
 */
UCLASS(Config = "GameplayInputSystemSettings", Defaultconfig,
	meta = (DisplayName = "Gameplay Input Forwarding System Settings"))
class GAMEPLAYINPUTFORWARDINGSYSTEM_API UGameplayInputForwardingSystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static UGameplayInputForwardingSystemSettings* Get()
	{
		return CastChecked<UGameplayInputForwardingSystemSettings>(
			UGameplayInputForwardingSystemSettings::StaticClass()->GetDefaultObject());
	}

	virtual FName GetContainerName() const override
	{
		return TEXT("Project");
	}

	virtual FName GetCategoryName() const override
	{
		return TEXT("GameplayInputSystem");
	}

	virtual FName GetSectionName() const override
	{
		return TEXT("GameplayInputForwardingSystem");
	}

#if WITH_EDITOR
	virtual FText GetSectionText() const override
	{
		return FText::FromString("Gameplay Input Forwarding System");
	}

	virtual FText GetSectionDescription() const override
	{
		return FText::FromString("Configure Gameplay Input Forwarding System");
	}
#endif

public:
	UPROPERTY(Config, EditAnywhere, Category = "Gameplay Input Forwarding System Settings",
		meta = (AllowedClasses = "/Script/GameplayInputForwardingSystem.GameplayInputForwardingMapping"))
	FSoftObjectPath DefaultGameplayInputForwardingMapping;
};
