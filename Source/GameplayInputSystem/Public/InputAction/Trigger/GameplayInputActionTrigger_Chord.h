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
	UPROPERTY(EditAnywhere, Category = "Gameplay Input Action Trigger")
	TArray<FGameplayInputSourceCommand> ComboCommands;

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Action Trigger")
	bool bInOrder = true;

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Action Trigger",
		meta=(EditAsFrames, ClampMin="0", UIMin="0", UIMax="2"))
	float ComboTimeout = 0.066667f;

protected:
	FTimerHandle ComboTimeoutHandle;

	virtual void OnTriggerBegin_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
	virtual void PreTriggerFinished_Implementation(bool bWasSuccessful, bool bCanceled = false) override;
	virtual bool ValidateTriggerCanFinish_Implementation() override;
	virtual bool CheckInputCommandCanBeCaptured_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;

	void OnComboTimeout();
};
