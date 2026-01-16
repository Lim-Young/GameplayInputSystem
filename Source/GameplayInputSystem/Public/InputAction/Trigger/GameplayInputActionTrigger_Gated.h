// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "InputAction/GameplayInputActionSet.h"
#include "GameplayInputActionTrigger_Gated.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Gated (门限指令)")
class GAMEPLAYINPUTSYSTEM_API UGameplayInputActionTrigger_Gated : public UGameplayInputActionTrigger
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FGameplayInputSourceCommand GateOpenCommand;

	UPROPERTY(EditAnywhere)
	FGameplayInputSourceCommand GateCloseCommand;

	UPROPERTY(EditAnywhere)
	FGameplayInputSourceCommand TriggerCommand;

	UPROPERTY(EditAnywhere)
	bool bCompleteOnGateClose = false;

	virtual bool
	CheckInputCommandCanBeCaptured_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
	virtual void OnInputCommandCaptured_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
};
