// Copyright Lim Young.


#include "GameplayInputSystemLibrary.h"

void UGameplayInputSystemLibrary::InjectGameplayInput(UObject* WorldContextObject, const FGameplayTag& InputSourceTag,
                                                      const EGameplayInputType InputType)
{
	const UWorld* World = WorldContextObject->GetWorld();
	if (World->GetNetMode() != NM_DedicatedServer)
	{
		if (UGameplayInputSubsystem* GameplayInputSubsystem = World->GetSubsystem<UGameplayInputSubsystem>())
		{
			GameplayInputSubsystem->InjectGameplayInput(InputSourceTag, InputType);
		}
	}
}
