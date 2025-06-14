// Copyright Lim Young.


#include "InteractionProcessor/GameplayInteractionProcessor_Mash.h"

void UGameplayInteractionProcessor_Mash::ResetMashCount()
{
	// Reset the mash count to zero after the input lifetime expires.
	CurrentMashCount = 0;
}

void UGameplayInteractionProcessor_Mash::CompleteMash()
{
	if (OwnerWorld->GetTimerManager().IsTimerActive(MashInputTimerHandle))
	{
		// If the timer is active, we clear it.
		OwnerWorld->GetTimerManager().ClearTimer(MashInputTimerHandle);
	}

	if (CurrentMashCount >= MinMashCount)
	{
		// If the mash count is greater than or equal to the minimum mash count, we complete the interaction.
		CompleteInteraction(true);
	}
	else
	{
		// If the mash count is less than the minimum mash count, we do not complete the interaction.
		CompleteInteraction(false);
	}
}

void UGameplayInteractionProcessor_Mash::PostInitialize_Implementation()
{
	CurrentMashCount = 0;

	if (!bCompleteEarly)
	{
		OwnerWorld->GetTimerManager().SetTimer(MashInteractionTimerHandle, this,
		                                       &UGameplayInteractionProcessor_Mash::CompleteMash,
		                                       MashDuration, false);
	}
}

void UGameplayInteractionProcessor_Mash::OnGameplayInputEvent_Implementation(const FGameplayTag& InputTag,
                                                                             const EGameplayInputType InputType)
{
	if (InputType == EGameplayInputType::Released)
	{
		// If the input is released, we do not complete the interaction.
		return;
	}

	if (InputTag.GetSingleTagContainer().HasAny(MashInputTags))
	{
		// If the input tag matches any of the mash input tags, we increment the mash count.
		CurrentMashCount++;

		if (OwnerWorld->GetTimerManager().IsTimerActive(MashInputTimerHandle))
		{
			// If the timer is already active, we reset it.
			OwnerWorld->GetTimerManager().ClearTimer(MashInputTimerHandle);
		}

		// If the mash count reaches the threshold, we complete the interaction.
		if (bCompleteEarly)
		{
			if (CurrentMashCount >= MinMashCount)
			{
				CompleteInteraction(true);
				return;
			}
		}

		OwnerWorld->GetTimerManager().SetTimer(MashInputTimerHandle, this,
		                                       &UGameplayInteractionProcessor_Mash::ResetMashCount,
		                                       MashInputLifetime, false);
	}
}

void UGameplayInteractionProcessor_Mash::PreCleanup_Implementation()
{
	if (!bCompleteEarly)
	{
		if (OwnerWorld->GetTimerManager().IsTimerActive(MashInteractionTimerHandle))
		{
			// If the timer is active, we clear it.
			OwnerWorld->GetTimerManager().ClearTimer(MashInteractionTimerHandle);
		}
	}

	if (OwnerWorld->GetTimerManager().IsTimerActive(MashInputTimerHandle))
	{
		// If the interaction timer is active, we clear it.
		OwnerWorld->GetTimerManager().ClearTimer(MashInputTimerHandle);
	}
}
