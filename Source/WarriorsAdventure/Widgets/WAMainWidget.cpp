// Fill out your copyright notice in the Description page of Project Settings.


#include "WAMainWidget.h"
#include "WARadarWidget.h"

UWARadarWidget* UWAMainWidget::GetRadarWidget()
{
	return RadarWidget;
}

void UWAMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
