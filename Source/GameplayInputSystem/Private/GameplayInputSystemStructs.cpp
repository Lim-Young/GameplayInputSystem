// Copyright Lim Young.


#include "GameplayInputSystemStructs.h"

bool FGameplayInputActionEventHandle::IsMatch(const FGameplayTag& InInputActionTag,
                                              const EGameplayInputActionEvent InInputActionEvent) const
{
	return InputActionTag == InInputActionTag && InputActionEvents >> 1 & static_cast<uint8>(InInputActionEvent);
}
