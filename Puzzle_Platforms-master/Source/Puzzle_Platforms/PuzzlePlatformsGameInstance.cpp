// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "ConstructorHelpers.h"
#include "TriggerPlatform.h"
#include "Blueprint/UserWidget.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/PlayerMenu.h"
#include "MenuSystem/LoadingMenu.h"

#include "OnlineSessionSettings.h"
#include "UnrealNames.h"


const static FName SESSION_NAME = NAME_GameSession;
const static FName SESSION_NAME_CONSTANT_KEY = (TEXT("SessionName"));


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
		UE_LOG(LogTemp, Warning, TEXT("Player Menu Class not found"));
		return;
	}

	PlayerMenuClass = PlayerMenuWidgetClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> LoadingMenuWidgetClass(TEXT("/Game/MenuSystem/WBP_LoadingMenu"));

	if (!ensure(LoadingMenuWidgetClass.Class))
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading Menu Class not found"));
		return;
	}

	LoadingMenuClass = LoadingMenuWidgetClass.Class;
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
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
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
		 TArray<FServerData> SessionDataFound;

		auto SearchResultsPool = SessionSearch->SearchResults;

		for (auto& Session : SearchResultsPool)
		{
			UE_LOG(LogTemp, Warning, TEXT("Session found: %s"), *Session.GetSessionIdStr());
			FServerData Data;
			Data.MaxPlayers = Session.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - Session.Session.NumOpenPublicConnections;
			Data.HostUserName = Session.Session.OwningUserName;

			FString SessionName;
			if (Session.Session.SessionSettings.Get(SESSION_NAME_CONSTANT_KEY, SessionName))
			{
				Data.Name = SessionName;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Session Test NOT Found"));
			}

			SessionDataFound.Add(Data);
		}

		Menu->SetServerList(SessionDataFound);
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

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/Lobby?listen");
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool bwasSuccesful)
{
	if (!bwasSuccesful) return;
	CreateSession(); //TODO Check what this is
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type ResultType)
{
	if (!ensure(SessionInterface)) return;

	FString ConnectInfo;

	if (SessionInterface->GetResolvedConnectString(SessionName, ConnectInfo))
	{
		UEngine* Engine = GetEngine();
		if (!ensure(Engine)) return;

		APlayerController* PC = GetFirstLocalPlayerController();
		if (!ensure(PC)) return;

		PC->ClientTravel(ConnectInfo, ETravelType::TRAVEL_Absolute);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not join session"));
	}
}


void UPuzzlePlatformsGameInstance::Host(FString SessionName_)
{
	if (!SessionInterface.IsValid()) return;

	DesiredSessionName = SessionName_;

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

	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		SessionSettings.bIsLANMatch = true;
	}
	else
	{
		SessionSettings.bIsLANMatch = false;
	}

	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUsesPresence = true;
	SessionSettings.Set(SESSION_NAME_CONSTANT_KEY, DesiredSessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UPuzzlePlatformsGameInstance::Join(uint32 Index_)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) return;

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index_]);

}

void UPuzzlePlatformsGameInstance::Refresh()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Finding session..."));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}

}

void UPuzzlePlatformsGameInstance::Leave()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;


	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC)) return;

	PC->ClientTravel("/Game/ThirdPersonCPP/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}


void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!ensure(MenuClass)) return;
	Menu = CreateWidget<UMainMenu>(this, MenuClass);

	if (!ensure(Menu)) return;
	Menu->Setup();
	Menu->SetMenuInterface(this);

}

void UPuzzlePlatformsGameInstance::LoadLoadingMenu()
{
	if (!ensure(LoadingMenuClass)) return;
	LoadingMenu = CreateWidget<ULoadingMenu>(this, LoadingMenuClass);

	if (!ensure(LoadingMenu)) return;
	LoadingMenu->Setup();
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
