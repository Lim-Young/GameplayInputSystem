// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "InputAction/GameplayInputActionSet.h"
#include "GameplayInputActionTrigger_Hold.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Hold (持续按住)")
class GAMEPLAYINPUTSYSTEM_API UGameplayInputActionTrigger_Hold : public UGameplayInputActionTrigger
{
	GENERATED_BODY()

private:
	bool bIsHolding = false;
	
public:
	UPROPERTY(EditAnywhere, meta = (Categories = "GameplayInput.InputSource"))
	FGameplayTag InputSourceTag;
	
	virtual bool
	CheckInputCommandCanBeCaptured_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
	virtual void OnInputCommandCaptured_Implementation(const FGameplayInputSourceCommand& InInputCommand) override;
};
