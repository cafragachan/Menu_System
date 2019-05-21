// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "ConstructorHelpers.h"
#include "TriggerPlatform.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/PlayerMenu.h"

#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"

const static FName SESSION_NAME = (TEXT("My Session Name"));


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

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("OSS found: %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();

		if (!SessionInterface.IsValid()) return;
		
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionComplete);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OSS not found"));

	}
}



void UPuzzlePlatformsGameInstance::OnFindSessionComplete(bool bwasSuccesful)
{
	if (bwasSuccesful && SessionSearch.IsValid() && Menu)
	{
		 TArray<FString> SessionNamesFound;

		auto SearchResultsPool = SessionSearch->SearchResults;

		for (auto& Session : SearchResultsPool)
		{
			UE_LOG(LogTemp, Warning, TEXT("Session found: %s"), *Session.GetSessionIdStr());
			SessionNamesFound.Add(Session.GetSessionIdStr());
		}

		//Menu->SetServerList(SessionNamesFound);
	}

}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool bwasSuccesful)
{
	if (!bwasSuccesful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create Session"));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;

	GetEngine()->AddOnScreenDebugMessage(0, 3.0f, FColor::Cyan, "Host Calling");

	UWorld* World = GetWorld();
	if (!ensure(World)) return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool bwasSuccesful)
{
	if (!bwasSuccesful) return;
	CreateSession();
}



void UPuzzlePlatformsGameInstance::Host()
{
	if (!SessionInterface.IsValid()) return;

	auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

	if (ExistingSession)
	{
		SessionInterface->DestroySession(SESSION_NAME);
	}
	else
	{
		CreateSession();
	}
	
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (!SessionInterface.IsValid()) return;

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;

	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UPuzzlePlatformsGameInstance::Join(FString & Address)
{
	Refresh();

	if (Menu)
	{
		Menu->SetServerList({ "Test1", "Test2" });
	}

	/*UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;

	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC)) return;

	PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);*/
}

void UPuzzlePlatformsGameInstance::Leave()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;


	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC)) return;

	PC->ClientTravel("/Game/ThirdPersonCPP/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::Refresh()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;
		UE_LOG(LogTemp, Warning, TEXT("Finding session..."));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}

}


void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!ensure(MenuClass)) return;
	Menu = CreateWidget<UMainMenu>(this, MenuClass);

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
