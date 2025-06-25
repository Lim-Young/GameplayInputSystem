// Copyright (c) Lim Young

#pragma once

#include "CoreMinimal.h"
#include "GameplayInteractorProcessor.h"
#include "GameplayInteractorProcessor_Overlap.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYINTERACTIONSYSTEM_API UGameplayInteractorProcessor_Overlap : public UGameplayInteractorProcessor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Gameplay Interaction")
	TArray<FName> OverlapComponentTags;

	virtual void PostInitialize_Implementation() override;
	virtual void Cleanup_Implementation() override;

private:
	UPROPERTY()
	TArray<TObjectPtr<UPrimitiveComponent>> ActiveOverlapComponents;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);
};
