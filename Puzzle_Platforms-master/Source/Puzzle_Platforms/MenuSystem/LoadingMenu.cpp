// Fill out your copyright notice in the Description page of Project Settings.

#include "LoadingMenu.h"

void ULoadingMenu::Setup()
{
	this->AddToViewport();
}

void ULoadingMenu::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	this->RemoveFromViewport();
}
