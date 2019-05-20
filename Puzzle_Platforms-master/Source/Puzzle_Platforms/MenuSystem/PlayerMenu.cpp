// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerMenu.h"

#include "Components/Button.h"

bool UPlayerMenu::Initialize()
{
	if(!Super::Initialize()) return false;

	if (!CancelButton) return false;
	CancelButton->OnClicked.AddDynamic(this, &UPlayerMenu::CancelOnClicked);

	if (!QuitButton) return false;
	QuitButton->OnClicked.AddDynamic(this, &UPlayerMenu::QuitOnClicked);

	return true;
}

void UPlayerMenu::Load()
{
	this->AddToViewport();

	UWorld* World = GetWorld();

	if (!ensure(World)) return;
	auto PC = World->GetFirstPlayerController();

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;
}

void UPlayerMenu::Unload()
{
	UWorld* World = GetWorld();

	if (!ensure(World)) return;
	auto PC = World->GetFirstPlayerController();

	PC->bShowMouseCursor = false;
	FInputModeGameOnly GameInputMode;
	PC->SetInputMode(GameInputMode);

	this->RemoveFromViewport();
}

void UPlayerMenu::QuitOnClicked()
{
	if (!ensure(MenuInterface)) return;
	MenuInterface->Leave();
}

void UPlayerMenu::CancelOnClicked()
{
	Unload();
}

void UPlayerMenu::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	Unload();
}
