// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInteractionProcessor.h"
#include "GameplayInteractionProcessor_Press.generated.h"

/**
 * This class is responsible for processing press input events in the Gameplay Interaction System.
 */
UCLASS(DisplayName = "Press")
class GAMEPLAYINTERACTIONSYSTEM_API UGameplayInteractionProcessor_Press : public UGameplayInteractionProcessor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag PressInputTag;

	UPROPERTY(EditAnywhere)
	bool bAccumulatePress = false;

	UPROPERTY(EditAnywhere)
	float Timeout = 0.0f;

private:
	FTimerHandle PressTimeoutHandle;
	void OnPressTimeout() const;

public:
	virtual void PostInitialize_Implementation() override;
	virtual void OnGameplayInputEvent_Implementation(const FGameplayTag& InputTag,
	                                                 const EGameplayInputType InputType) override;
	virtual void PreCleanup_Implementation() override;
};
