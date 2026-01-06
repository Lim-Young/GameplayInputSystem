// Copyright Lim Young.


#include "GameplayInputDocketFactory.h"

#include "GameplayInputSubsystem.h"

UGameplayInputDocketFactory::UGameplayInputDocketFactory()
{
	SupportedClass = UGameplayInputDocket::StaticClass();
	bCreateNew = true;
}

UObject* UGameplayInputDocketFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
                                                         EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UGameplayInputDocket>(InParent, InClass, InName, Flags, Context);
}
