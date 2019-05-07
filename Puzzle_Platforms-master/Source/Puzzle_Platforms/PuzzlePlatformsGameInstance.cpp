// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

void UPuzzlePlatformsGameInstance::Init()
{

}

void UPuzzlePlatformsGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;

	GetEngine()->AddOnScreenDebugMessage(0, 3.0f, FColor::Cyan, "Host Calling");

	UWorld* World = GetWorld();
	if (!ensure(World)) return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(FString & Address)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;

	GetEngine()->AddOnScreenDebugMessage(0, 3.0f, FColor::Green, FString::Printf(TEXT("Joining: %s"), *Address));

	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC)) return;

	PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}
