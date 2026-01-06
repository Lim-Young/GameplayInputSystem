// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "GameplayInputDocketFactory.generated.h"

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
