// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"

#include "PlayerMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLE_PLATFORMS_API UPlayerMenu : public UUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	UFUNCTION()
	void QuitOnClicked();

	UFUNCTION()
	void CancelOnClicked();

	IMenuInterface* MenuInterface;

public:

	virtual bool Initialize() override;

	void Load();

	void Unload();

	virtual void OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld) override;

	void SetMenuInterface(IMenuInterface* MenuInterface_) { MenuInterface = MenuInterface_; }
};
