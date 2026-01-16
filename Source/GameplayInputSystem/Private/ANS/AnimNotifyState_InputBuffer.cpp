// Copyright Lim Young.


#include "ANS/AnimNotifyState_InputBuffer.h"

#include "GameplayInputSubsystem.h"

void UAnimNotifyState_InputBuffer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!IsValid(InputBufferSchema))
	{
		return;
	}

	if (!IsValid(MeshComp) || !MeshComp->GetWorld() || MeshComp->GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	UGameplayInputSubsystem* GameplayInputSubsystem = MeshComp->GetWorld()->GetSubsystem<UGameplayInputSubsystem>();
	check(GameplayInputSubsystem);

	GameplayInputSubsystem->CreateAndRegisterGameplayInputBuffer(InputBufferSchema, BufferScope);
}

void UAnimNotifyState_InputBuffer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!IsValid(InputBufferSchema))
	{
		return;
	}

	if (!IsValid(MeshComp) || !MeshComp->GetWorld() || MeshComp->GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	UGameplayInputSubsystem* GameplayInputSubsystem = MeshComp->GetWorld()->GetSubsystem<UGameplayInputSubsystem>();
	check(GameplayInputSubsystem);

	GameplayInputSubsystem->FinishAndUnregisterGameplayInputBuffer(InputBufferSchema);
}
