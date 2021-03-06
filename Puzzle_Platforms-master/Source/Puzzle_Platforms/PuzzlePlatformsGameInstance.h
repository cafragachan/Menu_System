// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"

#include "PuzzlePlatformsGameInstance.generated.h"


const static FName SESSION_NAME = NAME_GameSession;
const static FName SESSION_NAME_CONSTANT_KEY = (TEXT("SessionName"));
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
	void LoadLoadingMenu();

	UFUNCTION(Exec, BlueprintCallable)
	void LoadMenu();

	UFUNCTION(Exec, BlueprintCallable)
	void CreatePlayerMenu();

	UFUNCTION(Exec, BlueprintCallable)
	void LoadPlayerMenu();

	UFUNCTION(Exec, BlueprintCallable)
	void UnloadPlayerMenu();

	UFUNCTION(Exec)
	virtual void Host(FString SessionName_) override;

	UFUNCTION(Exec)
	virtual void Join(uint32 Index_) override;

	UFUNCTION(Exec)
	virtual void Leave() override;

	UFUNCTION(Exec)
	virtual void Refresh() override;

	IOnlineSessionPtr GetSessionInterface() const { return SessionInterface; }

private:

	FString DesiredSessionName;

	class UPlayerMenu* PlayerMenu;
	class UMainMenu* Menu;
	class ULoadingMenu* LoadingMenu;

	TSubclassOf<class UUserWidget> LoadingMenuClass;
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> PlayerMenuClass;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	void OnFindSessionComplete(bool bwasSuccesful);
	void OnCreateSessionComplete(FName SessionName, bool bwasSuccesful);
	void OnDestroySessionComplete(FName SessionName, bool bwasSuccesful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type ResultType);

	void CreateSession();
};
