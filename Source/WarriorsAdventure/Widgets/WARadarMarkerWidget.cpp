// Fill out your copyright notice in the Description page of Project Settings.


#include "WARadarMarkerWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UWARadarMarkerWidget::UWARadarMarkerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_MAIN_QUEST_ICON(TEXT("Texture2D'/Game/RadarSystem/Textures/Icon_MainQuest.Icon_MainQuest'"));
	if (T_MAIN_QUEST_ICON.Succeeded())
	{
		MainQuestIcon = T_MAIN_QUEST_ICON.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_SIDE_QUEST_ICON(TEXT("Texture2D'/Game/RadarSystem/Textures/Icon_SideQuest.Icon_SideQuest'"));
	if (T_SIDE_QUEST_ICON.Succeeded())
	{
		SideQuestIcon = T_SIDE_QUEST_ICON.Object;
	}
}

void UWARadarMarkerWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (MarkerType == EMarkerType::MainQuest)
	{
		MarkerIcon->SetBrushFromTexture(MainQuestIcon);
	}
	else if (MarkerType == EMarkerType::SideQuest)
	{
		MarkerIcon->SetBrushFromTexture(SideQuestIcon);
	}
}

void UWARadarMarkerWidget::UpdateDistance(int32 Distance)
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