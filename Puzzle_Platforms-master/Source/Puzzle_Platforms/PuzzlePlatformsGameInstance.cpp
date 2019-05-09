// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "ConstructorHelpers.h"
#include "TriggerPlatform.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/PlayerMenu.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectIn)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	
	if (!ensure(MainMenuWidgetClass.Class))
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu Class not found"));
		return;
	}

	MenuClass = MainMenuWidgetClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> PlayerMenuWidgetClass(TEXT("/Game/MenuSystem/WBP_PlayerMenu"));

	if (!ensure(PlayerMenuWidgetClass.Class))
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu Class not found"));
		return;
	}

	PlayerMenuClass = PlayerMenuWidgetClass.Class;
}



void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();
	
	if (ensure(MenuClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Menu Class: %s"), *MenuClass->GetName());

	}
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!ensure(MenuClass)) return;
	
	UMainMenu* Menu = CreateWidget<UMainMenu>(this, MenuClass);

	if (!ensure(Menu)) return;

	Menu->Setup();
	Menu->SetMenuInterface(this);
	
}

void UPuzzlePlatformsGameInstance::CreatePlayerMenu()
{
	if (!ensure(PlayerMenuClass)) return;

	PlayerMenu = CreateWidget<UPlayerMenu>(this, PlayerMenuClass);
	
	if (!ensure(PlayerMenu)) return;

	PlayerMenu->Unload();
	PlayerMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadPlayerMenu()
{

	if (!ensure(PlayerMenu)) return;

	PlayerMenu->Load();

}

void UPuzzlePlatformsGameInstance::UnloadPlayerMenu()
{

	if (!ensure(PlayerMenu)) return;

	PlayerMenu->Unload();

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

	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC)) return;

	PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::Leave()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;


	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC)) return;

	PC->ClientTravel("/Game/ThirdPersonCPP/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}
