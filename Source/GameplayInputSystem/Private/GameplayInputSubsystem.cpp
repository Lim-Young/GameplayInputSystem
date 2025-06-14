// Copyright Lim Young.


#include "GameplayInputSubsystem.h"

UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayInput, "GameplayInput",
                               "Tag used to identify gameplay input actions. This tag is used to differentiate between gameplay-related inputs and other types of inputs.")

void UGameplayInputSubsystem::InjectGameplayInput(const FGameplayTag& InputTag, const EGameplayInputType InputType)
{
	if (!InputTag.MatchesTag(GameplayInput))
	{
		return;
	}

	OnGameplayInputEvent.Broadcast(InputTag, InputType);
}
