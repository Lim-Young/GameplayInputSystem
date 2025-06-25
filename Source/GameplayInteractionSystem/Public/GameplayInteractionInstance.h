// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputSubsystem.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Widgets/Layout/Anchors.h"
#include "GameplayInteractionInstance.generated.h"

class UGameplayInteractionProcessor;

USTRUCT(BlueprintType)
struct FGameplayInteractionWidgetConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Interaction",
		meta = (MustImplement = "/Script/GameplayInteractionSystem.GameplayInteractionWidgetInterface"))
	TSubclassOf<UUserWidget> InteractionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Interaction")
	FAnchors WidgetAnchors = FAnchors(0.5f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Interaction")
	FVector2D WidgetAlignment = FVector2D(0.5f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Interaction")
	FVector2D WidgetDesiredSize = FVector2D(64.0f);

	bool Valid() const;
};

USTRUCT(BlueprintType)
struct FGameplayInteractionDescription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TObjectPtr<UGameplayInteractionProcessor> InteractionProcessor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionInputEvent, const FGameplayTag&, InputTag,
	                                             const EGameplayInputType, InputType);

	UPROPERTY(BlueprintAssignable, Category = "Gameplay Interaction")
	FOnInteractionComplete OnInteractionSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Gameplay Interaction")
	FOnInteractionFailed OnInteractionFailed;

	UPROPERTY(BlueprintAssignable, Category = "Gameplay Interaction")
	FOnInteractionComplete OnInteractionComplete;

	UPROPERTY(BlueprintAssignable, Category = "Gameplay Interaction")
	FOnInteractionInputEvent OnInteractionInputEvent;

private:
	UPROPERTY()
	TObjectPtr<UGameplayInteractionProcessor> InteractionProcessor;

	UPROPERTY()
	TObjectPtr<UUserWidget> InteractionWidget;

public:
	virtual void BeginDestroy() override;
	void InitializeInteractionInstance(const FGameplayInteractionDescription& InteractionDescription);

	void DestroyInteractionInstance();

private:
	void Cleanup();
	void OnWorldCleanup(UWorld* World, bool bArg, bool bCond);
	void ProcessGameplayInputEvent(const FGameplayTag& InputTag, const EGameplayInputType InputType) const;
	void CompleteInteraction(const bool bSuccess);
};
