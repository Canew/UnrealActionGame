// Fill out your copyright notice in the Description page of Project Settings.


#include "WARadarEnemyMarkerWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UWARadarEnemyMarkerWidget::UWARadarEnemyMarkerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_ENEMY_ICON(TEXT("Texture2D'/Game/RadarSystem/Textures/Icon_Monster.Icon_Monster'"));
	if (T_ENEMY_ICON.Succeeded())
	{
		EnemyMarkerTexture = T_ENEMY_ICON.Object;
	}
}

void UWARadarEnemyMarkerWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (EnemyMarkerIcon)
	{
		EnemyMarkerIcon->SetBrushFromTexture(EnemyMarkerTexture);
	}
}

void UWARadarEnemyMarkerWidget::UpdateDistance(int32 Distance)
{
	Distance = FMath::Clamp<int32>(Distance, 0, MaxDisplayedDistance);

	FText DisplayedText;
	if (Distance > MaxDisplayedDistance)
	{
		DisplayedText = FText::FromString(FString::FromInt(MaxDisplayedDistance) + TEXT("+"));
	}
	else
	{
		DisplayedText = FText::FromString(FString::FromInt(Distance) + TEXT("m"));
	}

	DistanceText->SetText(DisplayedText);
}