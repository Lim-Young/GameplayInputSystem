// Copyright Lim Young.


#include "GameplayInteractionSystemLibrary.h"

UGameplayInteractionInstance* UGameplayInteractionSystemLibrary::CreateGameplayInteractionInstance(
	UObject* WorldContextObject, FGameplayInteractionDescription& Description,
	UWidgetComponent* WidgetComponent /*= nullptr*/)
{
	if (!IsValid(WorldContextObject))
	{
		UE_LOG(LogTemp, Error,
		       TEXT("UGameplayInteractionSystemLibrary::CreateGameplayInteractionInstance - Outer is not valid"));
		return nullptr;
	}

	if (UGameplayInteractionInstance* NewInstance = NewObject<UGameplayInteractionInstance>(WorldContextObject))
	{
		NewInstance->AddToRoot();
		NewInstance->InitializeInteractionInstance(Description, WidgetComponent);
		return NewInstance;
	}

	UE_LOG(LogTemp, Error,
	       TEXT("UGameplayInteractionSystemLibrary::CreateGameplayInteractionInstance - Failed to create instance"));
	return nullptr;
}
