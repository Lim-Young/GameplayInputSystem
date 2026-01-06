// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputDocket.h"
#include "UObject/Object.h"
#include "GameplayInputArbiter.generated.h"

/**
 * 
 */
UCLASS()
class UGameplayInputArbiter : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UGameplayInputDocket* GameplayInputDocker;

	UPROPERTY()
	TArray<TObjectPtr<UGameplayInputCommand>> GameplayInputCommandQueue;

public:
	void Initialize(UGameplayInputDocket* InGameplayInputDocker);

	void Start();
	void Cancel();
	bool Finish(UGameplayInputCommand*& ResultCommand);

	bool ReceiveGameplayInput(FGameplayTag InputTag, EGameplayInputType InputType);
};
