// Fill out your copyright notice in the Description page of Project Settings.


#include "WARadarLandmarkWidget.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"

void UWARadarLandmarkWidget::NativePreConstruct()
{
	FSlateBrush SlateBrush;
	SlateBrush.SetImageSize(FVector2D(LandmarkTexture->GetSizeX(), LandmarkTexture->GetSizeY()));
	SlateBrush.SetResourceObject(LandmarkTexture);
	LandmarkIcon->SetBrush(SlateBrush);
}

void UWARadarLandmarkWidget::SetLandmarkTexture(UTexture2D* Texture)
{
	LandmarkTexture = Texture;
}