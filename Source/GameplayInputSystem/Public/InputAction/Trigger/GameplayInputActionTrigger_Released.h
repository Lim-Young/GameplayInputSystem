// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "InputAction/GameplayInputActionSet.h"
#include "GameplayInputActionTrigger_Released.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Released (松开)")
class GAMEPLAYINPUTSYSTEM_API UGameplayInputActionTrigger_Released : public UGameplayInputActionTrigger
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (Categories = "GameplayInput.InputSource"))
	FGameplayTag InputSourceTag;

	virtual bool CheckInputCommandCanBeCaptured_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
	virtual bool ValidateTriggerCanFinish_Implementation() override;
};
