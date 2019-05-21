// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"

#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLE_PLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual bool Initialize() override;

	virtual void OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld) override;

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitch;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBar;

	//UPROPERTY(meta = (BindWidget))
	//class UEditableTextBox* IPAddressField;

	UPROPERTY(EditdefaultsOnly)
	TSubclassOf<class UTextWidget> TextWidgetBase;

	UFUNCTION()
	void HostOnClicked();

	UFUNCTION()
	void JoinOnClicked();
	
	UFUNCTION()
	void SwitchMenu();

	UFUNCTION()
	void BackMenu();

	UFUNCTION()
	void QuitGame();

	IMenuInterface* MenuInterface;

	TOptional<uint32> SelectedIndex;

public:

	void SetServerList(TArray<FString> ServerNames_);

	void SetMenuInterface(IMenuInterface* MenuInterface_) { MenuInterface = MenuInterface_; }

	void Setup();

	void SetSelectedIndex(uint32 Index_);
};
