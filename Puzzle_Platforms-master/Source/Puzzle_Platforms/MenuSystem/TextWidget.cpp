// Fill out your copyright notice in the Description page of Project Settings.

#include "TextWidget.h"
#include "MainMenu.h"
#include "Components/Button.h"



bool UTextWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	

	return true;
}

void UTextWidget::Setup(UMainMenu* Parent_, uint32 Index)
{
	MyIndex = Index;
	Parent = Parent_;

	if (!RowButton) return;

	RowButton->OnClicked.AddDynamic(this, &UTextWidget::TextWidgetOnClicked);
}

void UTextWidget::TextWidgetOnClicked()
{
	Parent->SetSelectedIndex(MyIndex);

}
