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
	bool bActionGatedMode = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bActionGatedMode", EditConditionHides))
	FGameplayInputSourceCommand GateOpenCommand;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bActionGatedMode", EditConditionHides))
	FGameplayInputSourceCommand GateCloseCommand;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bActionGatedMode", EditConditionHides))
	FGameplayTag GatedActionTag;

	UPROPERTY(EditAnywhere, meta = (Categories = "GameplayInput.InputSource"))
	FGameplayTag TriggerInputSourceTag;
	
private:
	bool bIsGateOpen = false;
	
protected:
	virtual void OnTriggerBegin_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
	virtual bool
	CheckInputCommandCanBeCaptured_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
	virtual void OnInputCommandCaptured_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
};
