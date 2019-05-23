// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "TextWidget.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!HostButton) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::SwitchToHostServerMenu);

	if (!HostGameButton) return false;
	HostGameButton->OnClicked.AddDynamic(this, &UMainMenu::HostOnClicked);

	if (!JoinButton) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::SwitchToJoinServerMenu);

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
	if (!ensure(MenuInterface && HostTextBox)) return;
	FString UserSessionName = HostTextBox->GetText().ToString();

	MenuInterface->Host(UserSessionName);
}

void UMainMenu::JoinOnClicked()
{

	if (SelectedIndex.IsSet() && MenuInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("Index server row: %i"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("index not set"));
	}


}

void UMainMenu::SetServerList(TArray<FServerData> ServerData_)
{
	if (ScrollBar && TextWidgetBase)
	{
		ScrollBar->ClearChildren();

		uint32 i = 0;
		for (auto& ServerData : ServerData_)
		{
			UTextWidget* TextWidget;
			TextWidget = CreateWidget<UTextWidget>(this, TextWidgetBase);

			TextWidget->ServerName->SetText(FText::FromString(ServerData.Name));
			TextWidget->HostName->SetText(FText::FromString(ServerData.HostUserName));
			TextWidget->CurrentPlayersFraction->SetText(FText::FromString(FString::Printf(TEXT("%i/%i"), ServerData.CurrentPlayers, ServerData.MaxPlayers)));

			TextWidget->Setup(this, i);

			ScrollBar->AddChild(TextWidget);
			++i;
		}
	}
}

void UMainMenu::SwitchToJoinServerMenu()
{
	if (!ensure(JoinMenu && WidgetSwitch)) return;

	WidgetSwitch->SetActiveWidget(JoinMenu);

	if (MenuInterface != nullptr) {
		MenuInterface->Refresh();
	}
}

void UMainMenu::SwitchToHostServerMenu()
{
	if (!ensure(HostMenu && WidgetSwitch)) return;

	WidgetSwitch->SetActiveWidget(HostMenu);

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

void UMainMenu::SetSelectedIndex(uint32 Index_)
{
	SelectedIndex = Index_;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	if (!ScrollBar) return;
	int32 ChildrenCount = ScrollBar->GetChildrenCount();

	for (int32 i = 0; i < ChildrenCount; ++i)
	{	
		auto TextRow = Cast<UTextWidget>(ScrollBar->GetChildAt(i));
		if (TextRow)
		{
			TextRow->bIsSelected = false;

			if (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i)
			{
				TextRow->bIsSelected = true;
			}
		}
	}
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