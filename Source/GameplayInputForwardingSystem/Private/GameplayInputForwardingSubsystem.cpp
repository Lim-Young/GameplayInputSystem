// Copyright Lim Young.


#include "GameplayInputForwardingSubsystem.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayInputForwardingSystemSettings.h"
#include "GameplayInputSubsystem.h"

void UGameplayInputForwardingSubsystem::PostInitialize()
{
	Super::PostInitialize();

	UGameplayInputForwardingMapping* DefaultMapping = Cast<UGameplayInputForwardingMapping>(
		UGameplayInputForwardingSystemSettings::Get()->DefaultGameplayInputForwardingMapping.TryLoad());
	if (DefaultMapping)
	{
		AddGameplayInputForwardingMapping(DefaultMapping);
	}

	GameplayInputSubsystem = GetWorld()->GetSubsystem<UGameplayInputSubsystem>();
	check(GameplayInputSubsystem);

	GameplayInputSubsystem->OnGameplayInputEvent.AddDynamic(
		this, &UGameplayInputForwardingSubsystem::HandleGameplayInputEvent);
}

void UGameplayInputForwardingSubsystem::HandleGameplayInputEvent(const FGameplayTag& InputTag,
                                                                 const EGameplayInputType InputType)
{
	UGameplayInputForwardingMapping* Mapping = nullptr;
	if (!TryGetInputForwardingMapping(Mapping))
	{
		return;
	}

	FGameplayTag InputForwardingTag;
	if (Mapping->TryGetInputForwardingTag(InputTag, InputType, InputForwardingTag))
	{
		if (!IsValid(CachedLocalPlayerPawn))
		{
			ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			if (LocalPlayer && !LocalPlayer->PlayerController)
			{
				CachedLocalPlayerPawn = LocalPlayer->PlayerController->GetPawn();
			}
		}

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CachedLocalPlayerPawn, InputForwardingTag,
		                                                         FGameplayEventData());
	}
}

void UGameplayInputForwardingSubsystem::AddGameplayInputForwardingMapping(UGameplayInputForwardingMapping* InMapping)
{
	if (GameplayInputForwardingMappings.Contains(InMapping))
	{
		return;
	}

	GameplayInputForwardingMappings.HeapPush(InMapping, FGameplayInputForwardingMappingPredicate());
}

void UGameplayInputForwardingSubsystem::RemoveGameplayInputForwardingMapping(UGameplayInputForwardingMapping* InMapping)
{
	GameplayInputForwardingMappings.RemoveSingle(InMapping);
}

bool UGameplayInputForwardingSubsystem::TryGetInputForwardingMapping(UGameplayInputForwardingMapping*& OutMapping) const
{
	if (GameplayInputForwardingMappings.Num() == 0)
	{
		return false;
	}

	OutMapping = GameplayInputForwardingMappings.HeapTop();
	return IsValid(OutMapping);
}
