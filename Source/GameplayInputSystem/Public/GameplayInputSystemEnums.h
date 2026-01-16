// Copyright Lim Young.

#pragma once

UENUM(BlueprintType)
enum class EGameplayInputType : uint8
{
	Pressed,
	Released
};

UENUM(BlueprintType, meta=(Bitflags))
enum class EGameplayInputActionState : uint8
{
	Idle = 0,
	Started = 1,
	Ongoing = 2,
	Completed = 4,
	Canceled = 8,
	Triggered = 16
};
