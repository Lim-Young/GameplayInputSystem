// Copyright Lim Young.


#include "GameplayInteractionSystemLibrary.h"

UGameplayInteractionInstance* UGameplayInteractionSystemLibrary::CreateGameplayInteractionInstance(
	UObject* WorldContextObject, FGameplayInteractionDescription& Description)
{
	if (!IsValid(WorldContextObject))
	{
		UE_LOG(LogTemp, Error,
		       TEXT("UGameplayInteractionSystemLibrary::CreateGameplayInteractionInstance - Outer is not valid"));
		return nullptr;
	}

	UGameplayInteractionInstance* NewInstance = NewObject<UGameplayInteractionInstance>(WorldContextObject);
	if (NewInstance)
	{
		NewInstance->InitializeInteractionInstance(Description);
		return NewInstance;
	}

	UE_LOG(LogTemp, Error,
	       TEXT("UGameplayInteractionSystemLibrary::CreateGameplayInteractionInstance - Failed to create instance"));
	return nullptr;
}
