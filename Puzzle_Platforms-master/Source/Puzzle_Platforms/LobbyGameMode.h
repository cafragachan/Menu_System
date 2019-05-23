// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Puzzle_PlatformsGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLE_PLATFORMS_API ALobbyGameMode : public APuzzle_PlatformsGameMode
{
	GENERATED_BODY()

public:


	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;

private:

	uint32 PlayersNum = 0;

	void PlayersServerTravel();

};
