// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!HostButton) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostOnClicked);

	if (!JoinButton) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::SwitchMenu);

	if (!JoinGameButton) return false;
	JoinGameButton->OnClicked.AddDynamic(this, &UMainMenu::JoinOnClicked);

	if (!CancelButton) return false;
	CancelButton->OnClicked.AddDynamic(this, &UMainMenu::BackMenu);

	if (!QuitButton) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	return true;
}

void UMainMenu::HostOnClicked()
{
	if (!ensure(MenuInterface)) return;
	MenuInterface->Host();
}

void UMainMenu::JoinOnClicked()
{
	if (!ensure(MenuInterface && IPAddressField)) return;

	FString IPString = IPAddressField->GetText().ToString();

	MenuInterface->Join(IPString);
}

void UMainMenu::SwitchMenu()
{
	if (!ensure(JoinMenu && WidgetSwitch)) return;

	WidgetSwitch->SetActiveWidget(JoinMenu);
}

void UMainMenu::BackMenu()
{
	if (!ensure(MainMenu && WidgetSwitch)) return;

	WidgetSwitch->SetActiveWidget(MainMenu);
}

void UMainMenu::Setup()
{
	this->AddToViewport();

	UWorld* World = GetWorld();

	if (!ensure(World)) return;
	auto PC = World->GetFirstPlayerController();

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	UWorld* World = GetWorld();

	if (!ensure(World)) return;
	auto PC = World->GetFirstPlayerController();

	PC->bShowMouseCursor = false;
	FInputModeGameOnly GameInputMode;
	PC->SetInputMode(GameInputMode);
}

void UMainMenu::QuitGame()
{
	UWorld* World = GetWorld();

	if (!ensure(World)) return;
	auto PC = World->GetFirstPlayerController();

	if (!PC) return;
	PC->ConsoleCommand("Quit");
}