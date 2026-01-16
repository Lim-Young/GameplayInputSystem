// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "InputAction/GameplayInputActionSet.h"
#include "GameplayInputActionTrigger_Chord.generated.h"

UCLASS(DisplayName = "Chord (复合指令)")
class UGameplayInputActionTrigger_Chord : public UGameplayInputActionTrigger
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FGameplayInputSourceCommand> ComboCommands;

	UPROPERTY(EditAnywhere)
	bool bInOrder = true;

	UPROPERTY(EditAnywhere, meta=(EditAsFrames, ClampMin="0", UIMin="0", UIMax="2"))
	float ComboTimeout = 0.066667f;

protected:
	FTimerHandle ComboTimeoutHandle;

	virtual void OnTriggerBegin_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
	virtual bool ValidateTriggerCanFinish_Implementation() override;
	virtual bool
	CheckInputCommandCanBeCaptured_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
	virtual void OnResetTrigger_Implementation() override;

	void OnComboTimeout();
};
