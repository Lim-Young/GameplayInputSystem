// Copyright Lim Young.


#include "InteractionProcessor/GameplayInteractionProcessor_Press.h"

void UGameplayInteractionProcessor_Press::OnPressTimeout() const
{
	CompleteInteraction(false);
}

void UGameplayInteractionProcessor_Press::PostInitialize_Implementation()
{
	if (Timeout > 0)
	{
		if (OwnerWorld->GetTimerManager().IsTimerActive(PressTimeoutHandle))
		{
			OwnerWorld->GetTimerManager().ClearTimer(PressTimeoutHandle);
		}

		OwnerWorld->GetTimerManager().SetTimer(PressTimeoutHandle, this,
		                                       &UGameplayInteractionProcessor_Press::OnPressTimeout, Timeout, false);
	}
}

void UGameplayInteractionProcessor_Press::OnGameplayInputEvent_Implementation(const FGameplayTag& InputTag,
                                                                              const EGameplayInputType InputType)
{
	if (InputType == EGameplayInputType::Released)
	{
		// If the input is released, we do not complete the interaction.
		return;
	}

	if (InputTag.MatchesTag(PressInputTag) && InputType == EGameplayInputType::Pressed)
	{
		CompleteInteraction(true);
	}
	else
	{
		if (bAccumulatePress)
		{
			CompleteInteraction(false);
		}
	}
}

void UGameplayInteractionProcessor_Press::PreCleanup_Implementation()
{
	if (!(Timeout > 0))
	{
		return;
	}

	if (OwnerWorld->GetTimerManager().IsTimerActive(PressTimeoutHandle))
	{
		OwnerWorld->GetTimerManager().ClearTimer(PressTimeoutHandle);
	}
}
