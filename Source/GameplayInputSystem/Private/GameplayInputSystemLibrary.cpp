// Copyright Lim Young.


#include "GameplayInputSystemLibrary.h"

void UGameplayInputSystemLibrary::InjectGameplayInput(UObject* WorldContextObject, const FGameplayTag& InputTag,
                                                      const EGameplayInputType InputType)
{
	const UWorld* World = WorldContextObject->GetWorld();
	if (World->GetNetMode() != NM_DedicatedServer)
	{
		if (UGameplayInputSubsystem* GameplayInputSubsystem = World->GetSubsystem<UGameplayInputSubsystem>())
		{
			GameplayInputSubsystem->InjectGameplayInput(InputTag, InputType);
		}
	}
}
