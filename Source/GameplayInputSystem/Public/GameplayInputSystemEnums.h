// Copyright Lim Young.

#pragma once

UENUM(BlueprintType)
enum class EGameplayInputType : uint8
{
	Pressed,
	Released
};

UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EGameplayInputActionEvent : uint8
{
	None = 0 UMETA(Hidden),
	Started = 1,
	Ongoing = 2,
	Completed = 4,
	Canceled = 8,
	Triggered = 16
};

UENUM(BlueprintType)
enum class EGameplayInputActionState : uint8
{
	Inactive,
	Pending,
	Active
};
