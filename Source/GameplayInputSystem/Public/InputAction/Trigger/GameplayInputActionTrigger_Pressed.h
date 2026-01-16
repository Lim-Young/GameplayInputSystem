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
	UPROPERTY(EditAnywhere, meta = (Categories = "GameplayInput.InputSource"))
	FGameplayTag InputSourceTag;

	virtual bool CheckInputCommandCanBeCaptured_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
	virtual void OnInputCommandCaptured_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
};
