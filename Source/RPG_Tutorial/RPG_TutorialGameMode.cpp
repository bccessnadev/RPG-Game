// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_TutorialGameMode.h"
#include "RPG_TutorialCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARPG_TutorialGameMode::ARPG_TutorialGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/RPG_Character/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
