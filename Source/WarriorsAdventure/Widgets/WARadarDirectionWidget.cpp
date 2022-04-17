// Fill out your copyright notice in the Description page of Project Settings.


#include "WARadarDirectionWidget.h"
#include "Components/TextBlock.h"

void UWARadarDirectionWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	FText DisplayText = DirectionInfo.Name;
	FString DisplayString = DisplayText.ToString().LeftChop(DisplayText.ToString().Len() - 2).ToUpper();
	DisplayText = FText::FromString(DisplayString);
	DirectionText->SetText(DisplayText);
	DirectionText->SetOpacity(0.7f);
}