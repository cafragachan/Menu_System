// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLE_PLATFORMS_API UTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentPlayersFraction;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostName;

	UPROPERTY(meta = (BindWidget))
	class UButton* RowButton;

	UFUNCTION()
	void TextWidgetOnClicked();

	UPROPERTY(BlueprintReadOnly)
	bool bIsSelected = false;

	void Setup(class UMainMenu* Parent_, uint32 Index);


private:

	uint32 MyIndex;
	class UMainMenu* Parent;
};
