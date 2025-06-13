// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayInteractionInstance.generated.h"

class UGameplayInteractionProcessor;

USTRUCT(BlueprintType)
struct FGameplayInteractionWidgetConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Interaction")
	TSubclassOf<UUserWidget> InteractionWidgetClass;
};

USTRUCT(BlueprintType)
struct FGameplayInteractionDescription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UGameplayInteractionProcessor> InteractionProcessor;

	UPROPERTY(EditAnywhere)
	FGameplayInteractionWidgetConfig WidgetConfig;

	bool Valid() const;
};

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEPLAYINTERACTIONSYSTEM_API UGameplayInteractionInstance : public UObject
{
	GENERATED_BODY()

	friend class UGameplayInteractionProcessor;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionSuccess);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionFailed);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionComplete);

	UPROPERTY(BlueprintAssignable, Category = "Gameplay Interaction")
	FOnInteractionComplete OnInteractionSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Gameplay Interaction")
	FOnInteractionFailed OnInteractionFailed;

	UPROPERTY(BlueprintAssignable, Category = "Gameplay Interaction")
	FOnInteractionComplete OnInteractionComplete;

private:
	UPROPERTY()
	TObjectPtr<UGameplayInteractionProcessor> InteractionProcessor;

public:
	virtual void BeginDestroy() override;
	void InitializeInteractionInstance(const FGameplayInteractionDescription& InteractionDescription);

private:
	void Cleanup();
	void CompleteInteraction(const bool bSuccess);
};
