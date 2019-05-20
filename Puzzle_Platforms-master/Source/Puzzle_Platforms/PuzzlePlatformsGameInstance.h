// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLE_PLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

protected:

	virtual void Init() override;

public:

	UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectIn);

	UFUNCTION(Exec, BlueprintCallable)
	void LoadMenu();

	UFUNCTION(Exec, BlueprintCallable)
	void CreatePlayerMenu();

	UFUNCTION(Exec, BlueprintCallable)
	void LoadPlayerMenu();

	UFUNCTION(Exec, BlueprintCallable)
	void UnloadPlayerMenu();

	UFUNCTION(Exec)
	virtual void Host() override;

	UFUNCTION(Exec)
	virtual void Join(FString& Address) override;

	UFUNCTION(Exec)
	virtual void Leave() override;

	UFUNCTION(Exec)
	virtual void Refresh() override;

private:

	class UPlayerMenu* PlayerMenu;
	class UMainMenu* Menu;

	TSubclassOf<class UUserWidget> MenuClass;

	TSubclassOf<class UUserWidget> PlayerMenuClass;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch = nullptr;

	void OnFindSessionComplete(bool bwasSuccesful);
	void OnCreateSessionComplete(FName SessionName, bool bwasSuccesful);
	void OnDestroySessionComplete(FName SessionName, bool bwasSuccesful);
	void CreateSession();
};
