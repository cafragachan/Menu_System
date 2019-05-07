// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Puzzle_PlatformsGameMode.h"
#include "Puzzle_PlatformsCharacter.h"
#include "UObject/ConstructorHelpers.h"

APuzzle_PlatformsGameMode::APuzzle_PlatformsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
