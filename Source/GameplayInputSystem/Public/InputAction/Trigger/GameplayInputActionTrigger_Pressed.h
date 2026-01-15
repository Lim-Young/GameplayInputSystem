// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "InputAction/GameplayInputActionSet.h"
#include "GameplayInputActionTrigger_Pressed.generated.h"

UCLASS(DisplayName = "Pressed (按下)")
class UGameplayInputActionTrigger_Pressed : public UGameplayInputActionTrigger
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Gameplay Input Action Trigger")
	FGameplayInputCommand PressedCommand;

	virtual bool CheckInputCommandCanBeCaptured_Implementation(const FGameplayInputCommand& InInputCommand) override;
	virtual bool ValidateTriggerCanFinish_Implementation() override;
};
