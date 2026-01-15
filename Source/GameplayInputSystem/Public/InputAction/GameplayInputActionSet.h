// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputCommand.h"
#include "GameplayInputSystemEnums.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "GameplayInputActionSet.generated.h"

class UGameplayInputSubsystem;
class UGameplayInputAction;

UCLASS(Abstract, BlueprintType, DefaultToInstanced, EditInlineNew)
class UGameplayInputActionTrigger : public UObject
{
	GENERATED_BODY()

	friend class UGameplayInputActionSet;

protected:
	bool bReleaseInputCommands = false;

	UPROPERTY()
	UGameplayInputAction* OwningInputAction = nullptr;

	TArray<FGameplayInputCommand> CapturedInputCommands;

public:
	bool CaptureInputCommand(const FGameplayInputCommand& InInputCommand);

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Input Action Trigger")
	bool CheckInputCommandCanBeCaptured(const FGameplayInputCommand& InInputCommand);

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Input Action Trigger")
	bool ValidateTriggerCanFinish();

	void BeginTrigger(const FGameplayInputCommand& InInputCommand);

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Input Action Trigger")
	void OnTriggerBegin(const FGameplayInputCommand& InInputCommand);

	void ResetTrigger();

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Input Action Trigger")
	void OnResetTrigger();

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Input Action Trigger")
	void PreTriggerFinished(bool bWasSuccessful, bool bCanceled = false);

protected:
	UFUNCTION(BlueprintCallable, Category = "Gameplay Input Action Trigger")
	void FinishTrigger(bool bWasSuccessful, bool bCanceled = false);

	FTimerManager& GetTimerManager() const;

public:
	const TArray<FGameplayInputCommand>& GetCapturedInputCommands() const;
	bool IsReleasingInputCommands() const;
	void SetReleaseInputCommands(bool bInReleaseInputCommands);
};

#pragma region TriggerExamples


#pragma endregion

UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class UGameplayInputAction : public UObject
{
	GENERATED_BODY()

	friend class UGameplayInputActionSet;

public:
	UPROPERTY(EditAnywhere, Category = "Gameplay Input Action", meta = (Categories = GameplayInput_Action))
	FGameplayTag ActionTag;

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Action", Instanced,
		meta = (Categories = GameplayInput, TitleProperty = "{InputTag}[{InputType}]"))
	TArray<TObjectPtr<UGameplayInputActionTrigger>> Triggers;

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Action")
	uint8 Priority = 0;

private:
	EGameplayInputActionState CurrentActionState = EGameplayInputActionState::Idle;

	UPROPERTY()
	UGameplayInputActionSet* OwningActionSet = nullptr;

public:
	bool CheckCanActivateAction(const FGameplayInputCommand& InInputCommand);
	void SetActionState(EGameplayInputActionState NewActionState, bool bBroadcastEvent = true);
	void FinishAction(UGameplayInputActionTrigger* ExecutingTrigger, bool bWasSuccessful, bool bCanceled = false);

	void BeginAction(const FGameplayInputCommand& InInputCommand);
};

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEPLAYINPUTSYSTEM_API UGameplayInputActionSet : public UObject
{
	GENERATED_BODY()

	friend class UGameplayInputAction;
	friend class UGameplayInputActionTrigger;

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "Gameplay Input Action Set",
		meta = (TitleProperty = "{ActionTag} [{Priority}]"))
	TArray<TObjectPtr<UGameplayInputAction>> Actions;

	uint8 Priority = 0;

	static UGameplayInputActionSet* CreateByTemplateObject(const UGameplayInputActionSet* TemplateObject,
	                                                       UObject* Outer,
	                                                       UGameplayInputSubsystem* OwningSubsystem,
	                                                       uint8 InPriority = 0);

private:
	UPROPERTY()
	TObjectPtr<UGameplayInputSubsystem> OwningSubsystem = nullptr;

public:
	bool HandleInput(const FGameplayInputCommand& InInputCommand);

private:
	void TriggerGameplayInputAction(const FGameplayTag& ActionTag,
	                                const EGameplayInputActionState ActionState) const;
};
