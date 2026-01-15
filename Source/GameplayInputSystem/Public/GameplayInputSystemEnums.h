// Copyright Lim Young.

#pragma once

UENUM(BlueprintType)
enum class EGameplayInputType : uint8
{
	Pressed,
	Released
};

UENUM(BlueprintType)
enum class EGameplayInputActionState : uint8
{
	Idle,
	Started,
	Ongoing,
	Completed,
	Canceled
};
