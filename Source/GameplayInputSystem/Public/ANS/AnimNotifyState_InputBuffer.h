// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputArbiter.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_InputBuffer.generated.h"

class UGameplayInputDocket;

/**
 * 
 */
UCLASS(DisplayName = "Input Buffer")
class GAMEPLAYINPUTSYSTEM_API UAnimNotifyState_InputBuffer : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Input Buffer")
	TObjectPtr<UGameplayInputDocket> GameplayInputDocket;

	UPROPERTY(EditAnywhere, Category = "Input Buffer")
	EArbiterCommandMatchMode MatchMode = EArbiterCommandMatchMode::Both;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;
};
