// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLE_PLATFORMS_API ULoadingMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void Setup();

	void OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld) override;
};
