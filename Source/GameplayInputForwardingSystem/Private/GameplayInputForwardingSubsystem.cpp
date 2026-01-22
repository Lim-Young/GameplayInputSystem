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

	GameplayInputSubsystem->OnGameplayInputSourceEvent.AddDynamic(
		this, &UGameplayInputForwardingSubsystem::HandleGameplayInputSourceEvent);
	GameplayInputSubsystem->OnGameplayInputActionTriggered.AddDynamic(
		this, &UGameplayInputForwardingSubsystem::HandleGameplayInputActionEvent);
}

void UGameplayInputForwardingSubsystem::HandleGameplayInputSourceEvent(const FGameplayTag& InputTag,
                                                                       const EGameplayInputType InputType)
{
	UGameplayInputForwardingMapping* Mapping = nullptr;
	if (!TryGetInputForwardingMapping(Mapping))
	{
		return;
	}

	FGameplayTag InputForwardingTag;
	if (Mapping->TryGetInputSourceForwardingTag(InputTag, InputType, InputForwardingTag))
	{
		if (!IsValid(CachedLocalPlayerPawn))
		{
			const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			if (LocalPlayer && LocalPlayer->PlayerController)
			{
				CachedLocalPlayerPawn = LocalPlayer->PlayerController->GetPawn();
			}
		}

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CachedLocalPlayerPawn, InputForwardingTag,
		                                                         FGameplayEventData());
	}
}

void UGameplayInputForwardingSubsystem::HandleGameplayInputActionEvent(const FGameplayTag& InputActionTag,
                                                                       const EGameplayInputActionEvent ActionState)
{
	UGameplayInputForwardingMapping* Mapping = nullptr;
	if (!TryGetInputForwardingMapping(Mapping))
	{
		return;
	}

	FGameplayTag InputForwardingTag;
	if (Mapping->TryGetInputActionForwardingTag(InputActionTag, ActionState, InputForwardingTag))
	{
		if (!IsValid(CachedLocalPlayerPawn))
		{
			const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			if (LocalPlayer && LocalPlayer->PlayerController)
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
