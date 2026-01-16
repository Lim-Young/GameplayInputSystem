// Copyright Lim Young.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInputBuffer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_InputBuffer.generated.h"

class UGameplayInputBufferSchema;

/**
 * 
 */
UCLASS(DisplayName = "Input Buffer")
class GAMEPLAYINPUTSYSTEM_API UAnimNotifyState_InputBuffer : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Input Buffer")
	TObjectPtr<UGameplayInputBufferSchema> InputBufferSchema;

	UPROPERTY(EditAnywhere, Category = "Input Buffer")
	EGameplayInputBufferScope BufferScope = EGameplayInputBufferScope::Both;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;
};
