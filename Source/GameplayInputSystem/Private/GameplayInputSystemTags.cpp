// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayInputSystemTags.h"

UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayInput, "GameplayInput",
                               "Gameplay Input System Root Tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayInput_Source, "GameplayInput.Source",
                               "Identifies the source of the gameplay input, This will be mapping to different input devices or systems.")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayInput_Action, "GameplayInput.Action",
                               "Identifies a gameplay input action, This will be mapping to different gameplay input action sets.")
