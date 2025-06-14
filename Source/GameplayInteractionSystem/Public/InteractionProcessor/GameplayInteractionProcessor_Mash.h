// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInteractionProcessor.h"
#include "GameplayInteractionProcessor_Mash.generated.h"

/**
 * This class implements a mash interaction processor for the Gameplay Interaction System.
 */
UCLASS(DisplayName = "Mash")
class GAMEPLAYINTERACTIONSYSTEM_API UGameplayInteractionProcessor_Mash : public UGameplayInteractionProcessor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Interaction|Mash")
	FGameplayTagContainer MashInputTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Interaction|Mash",
		meta = (ClampMin = 1, UIMin = 1))
	int32 MinMashCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Interaction|Mash", meta = (ClampMin = 0))
	float MashDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Interaction|Mash",
		meta = (ClampMin = 0))
	float MashInputLifetime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Interaction|Mash")
	bool bCompleteEarly = false;

private:
	int32 CurrentMashCount = 0;
	FTimerHandle MashInteractionTimerHandle;
	FTimerHandle MashInputTimerHandle;

	void ResetMashCount();
	void CompleteMash();

public:
	virtual void PostInitialize_Implementation() override;
	virtual void OnGameplayInputEvent_Implementation(const FGameplayTag& InputTag,
	                                                 const EGameplayInputType InputType) override;
	virtual void PreCleanup_Implementation() override;
};
