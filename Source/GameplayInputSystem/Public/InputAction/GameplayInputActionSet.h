// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputSourceCommand.h"
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

	TArray<FGameplayInputSourceCommand> CapturedInputCommands;

public:
	bool CaptureInputCommand(const FGameplayInputSourceCommand& InInputCommand);

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Input Action Trigger")
	bool CheckCanBeginTrigger(const FGameplayInputSourceCommand& InInputCommand);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Input Action Trigger")
	bool CheckInputCommandCanBeCaptured(const FGameplayInputSourceCommand& InInputCommand);

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Input Action Trigger")
	void OnInputCommandCaptured(const FGameplayInputSourceCommand& InInputCommand);

	// /**
	//  * 该函数将在触发器开始后自动调用，用于验证触发器是否可以立即完成。
	//  * 若返回true，触发器将立即完成；若返回false，触发器将继续其执行流程，你需要自行维护触发器的状态，并在适当时机调用FinishTrigger函数以结束触发器。
	//  * 
	//  * This function will be called automatically after the trigger begins to validate whether the trigger can finish immediately.
	//  * If it returns true, the trigger will finish immediately; if it returns false, the trigger will continue its execution flow,
	//  * and you need to maintain the trigger's state yourself and call the FinishTrigger function at the appropriate time to end the trigger.
	//  * @return True if the trigger can finish immediately, false if it needs to continue executing.
	//  */
	// UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Input Action Trigger")
	// bool ValidateTriggerCanFinish();

	void BeginTrigger(const FGameplayInputSourceCommand& InInputCommand);

	/**
	 * 该函数在触发器开始时调用。
	 * 你可以使用此事件来初始化任何必要的变量或状态，或设置一些定时器来控制触发器的流程。
	 *
	 * This event is called when the trigger begins.
	 * You can use this event to initialize any necessary variables or states.
	 * Or set some timers to control the trigger flow.
	 * @param InInputCommand The input command that triggered this action.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Input Action Trigger")
	void OnTriggerBegin(const FGameplayInputSourceCommand& InInputCommand);

	void ResetTrigger();

	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Input Action Trigger")
	void OnResetTrigger();

	/**
	 * 当 FinishTrigger 被调用时，会先触发该回调事件，这是你最后一次机会在触发器所属的动作完成之前执行任何清理或状态更新操作。
	 * 一般情况下，若你的相关逻辑无需依赖触发器所属动作的状态变化，你可以选择在 OnResetTrigger 中处理清理工作。
	 * 
	 * This callback event is triggered when FinishTrigger is called, which is your last chance to perform any cleanup or state update operations before the action to which the trigger belongs is completed.
	 * In general, if your related logic does not need to rely on the state changes of the action to which the trigger belongs, you can choose to handle the cleanup work in OnResetTrigger.
	 * @param bWasSuccessful True if the action was completed successfully, false if it failed.
	 * @param bCanceled True if the action was canceled, valid only if bWasSuccessful is false.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Gameplay Input Action Trigger")
	void PreTriggerFinished(bool bWasSuccessful, bool bCanceled = false);

protected:
	UFUNCTION(BlueprintCallable, Category = "Gameplay Input Action Trigger")
	void FinishTrigger(bool bWasSuccessful, bool bCanceled = false);

	FTimerManager& GetTimerManager() const;

public:
	const TArray<FGameplayInputSourceCommand>& GetCapturedInputCommands() const;
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
	friend class UGameplayInputActionTrigger;

public:
	UPROPERTY(EditAnywhere, Category = "Gameplay Input Action", meta = (Categories = "GameplayInput.InputAction"))
	FGameplayTag InputActionTag;

	UPROPERTY(EditAnywhere, Instanced, Category = "Gameplay Input Action")
	TArray<TObjectPtr<UGameplayInputActionTrigger>> Triggers;

	UPROPERTY(EditAnywhere, Category = "Gameplay Input Action")
	uint8 Priority = 0;

private:
	EGameplayInputActionState CurrentActionState = EGameplayInputActionState::Idle;

	UPROPERTY()
	UGameplayInputActionSet* OwningActionSet = nullptr;

public:
	bool CheckCanActivateAction(const FGameplayInputSourceCommand& InInputCommand);
	void SetActionState(EGameplayInputActionState NewActionState, bool bBroadcastEvent = true);
	EGameplayInputActionState GetCurrentActionState() const;
	void BroadcastActionStateEvent(EGameplayInputActionState ActionState) const;
	void FinishAction(UGameplayInputActionTrigger* ExecutingTrigger, bool bWasSuccessful, bool bCanceled = false);

	void BeginAction(const FGameplayInputSourceCommand& InInputCommand);
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
		meta = (TitleProperty = "{InputActionTag} [P {Priority}]"))
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
	bool HandleInput(const FGameplayInputSourceCommand& InInputCommand,
	                 bool bClampedByHigherPriority = false,
	                 uint8 CustomPriority = -1);

private:
	void TriggerGameplayInputAction(const FGameplayTag& InputActionTag,
	                                const EGameplayInputActionState ActionState) const;

#if WITH_EDITOR

public:
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
};
