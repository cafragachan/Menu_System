// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"

#include "SoftObjectPath.h"
#include "GameMapsSettings.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "PuzzlePlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayersNum++;

	if (PlayersNum >= 2)
	{
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer, this, &ALobbyGameMode::PlayersServerTravel, 25.0f, false);
		
	}
}

void ALobbyGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);

	PlayersNum--;
}

void ALobbyGameMode::PlayersServerTravel()
{
	UWorld* World = GetWorld();
	if (!ensure(World)) return;

	bUseSeamlessTravel = true;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");

	auto PPGameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());

	if (PPGameInstance)
	{
		IOnlineSessionPtr SessionInterface = PPGameInstance->GetSessionInterface();
		SessionInterface->StartSession(SESSION_NAME);
	}
	
}
