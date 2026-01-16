// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction/GameplayInputActionSet.h"
#include "GameplayInputActionTrigger_MultiGated.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Multi Gated (复合门限指令)")
class GAMEPLAYINPUTSYSTEM_API UGameplayInputActionTrigger_MultiGated : public UGameplayInputActionTrigger
{
	GENERATED_BODY()

private:
	bool bIsGateOpen;
	bool bShouldFinishTrigger;

public:
	// UPROPERTY(EditAnywhere)
	// TArray<FGameplayInputSourceCommand> GateOpenCommands;
	UPROPERTY(EditAnywhere, meta=(Categories = "GameplayInput.InputSource"))
	TArray<FGameplayTag> GatesTags;

	// UPROPERTY(EditAnywhere)
	// TArray<FGameplayInputSourceCommand> GateCloseCommands;
	// UPROPERTY(EditAnywhere, meta=(Categories = "GameplayInput.InputSource"))
	// TArray<FGameplayTag> ClosedGatesTags;

	UPROPERTY(EditAnywhere)
	FGameplayInputSourceCommand TriggerCommand;
	
	UPROPERTY(EditAnywhere)
	bool bAllGatesRequired = true;
	
	UPROPERTY(EditAnywhere)
	bool bInOrder = false;
	
	virtual bool CheckCanBeginTrigger_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
	virtual void OnTriggerBegin_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
	virtual bool
	CheckInputCommandCanBeCaptured_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
	virtual void OnInputCommandCaptured_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
	virtual void OnResetTrigger_Implementation() override;
};
