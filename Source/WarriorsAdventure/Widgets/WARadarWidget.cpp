// Fill out your copyright notice in the Description page of Project Settings.


#include "WARadarWidget.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/Image.h"

UWARadarWidget::UWARadarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_RADAR_DIRECTION(TEXT("WidgetBlueprint'/Game/RadarSystem/Widgets/UI_RadarDirection.UI_RadarDirection_C'"));
	if (UI_RADAR_DIRECTION.Succeeded())
	{
		RadarDirectionWidgetClass = UI_RADAR_DIRECTION.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_RADAR_MARKER(TEXT("WidgetBlueprint'/Game/RadarSystem/Widgets/UI_RadarMarker.UI_RadarMarker_C'"));
	if (UI_RADAR_MARKER.Succeeded())
	{
		RadarMarkerWidgetClass = UI_RADAR_MARKER.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_RADAR_ENEMY_MARKER(TEXT("WidgetBlueprint'/Game/RadarSystem/Widgets/UI_RadarEnemyMarker1.UI_RadarEnemyMarker1_C'"));
	if (UI_RADAR_ENEMY_MARKER.Succeeded())
	{	
		RadarEnemyMarkerWidgetClass = UI_RADAR_ENEMY_MARKER.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_RADAR_LANDMARK(TEXT("WidgetBlueprint'/Game/RadarSystem/Widgets/UI_RadarLandmark.UI_RadarLandmark_C'"));
	if (UI_RADAR_LANDMARK.Succeeded())
	{
		RadarLandmarkWidgetClass = UI_RADAR_LANDMARK.Class;
	}

}

UWidgetAnimation* UWARadarWidget::GetBlendOutAnim()
{
	return BlendOut;
}

UUserWidget* UWARadarWidget::AddDirection(FDirectionInfo Info)
{
	APlayerController* PlayerController = GetOwningPlayer();
	UWARadarDirectionWidget* RadarDirectionWidget = CreateWidget<UWARadarDirectionWidget>(PlayerController, RadarDirectionWidgetClass);
	if (RadarDirectionWidget)
	{
		RadarDirectionWidget->SetDirectionInfo(Info);
		DirectionWidgets.Add(RadarDirectionWidget);

		UOverlaySlot* OverlaySlot = RadarOverlay->AddChildToOverlay(RadarDirectionWidget);
		OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);

		return RadarDirectionWidget;
	}
	else
	{
		return nullptr;
	}
}

UUserWidget* UWARadarWidget::AddMarker(EMarkerType Type)
{
	APlayerController* PlayerController = GetOwningPlayer();
	UWARadarMarkerWidget* RadarMarkerWidget = CreateWidget<UWARadarMarkerWidget>(PlayerController, RadarMarkerWidgetClass);
	if (RadarMarkerWidget)
	{
		RadarMarkerWidget->SetMarkerType(Type);
		MarkerWidgets.Add(RadarMarkerWidget);

		UOverlaySlot* OverlaySlot = RadarOverlay->AddChildToOverlay(RadarMarkerWidget);
		OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
		OverlaySlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));

		return RadarMarkerWidget;
	}
	else
	{
		return nullptr;
	}
}

UUserWidget* UWARadarWidget::AddLandmark(UTexture2D* Texture)
{
	APlayerController* PlayerController = GetOwningPlayer();
	UWARadarLandmarkWidget* RadarLandmarkWidget = CreateWidget<UWARadarLandmarkWidget>(PlayerController, RadarLandmarkWidgetClass);
	if (RadarLandmarkWidget)
	{
		RadarLandmarkWidget->SetLandmarkTexture(Texture);
		LandmarkWidgets.Add(RadarLandmarkWidget);

		UOverlaySlot* OverlaySlot = RadarOverlay->AddChildToOverlay(RadarLandmarkWidget);
		OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);

		return RadarLandmarkWidget;
	}
	else
	{
		return nullptr;
	}
}

void UWARadarWidget::HighlightRadar(bool Highlight)
{
	if (!RadarBody)
	{
		return;
	}

	FLinearColor RadarBodyColor;
	if (Highlight)
	{
		RadarBodyColor.R = 1.0f;
		RadarBodyColor.G = 0.75f;
		RadarBodyColor.B = 0.0f;
		RadarBodyColor.A = 1.0f;
		RadarBody->SetColorAndOpacity(RadarBodyColor);
	}
	else
	{
		RadarBodyColor.R = 1.0f;
		RadarBodyColor.G = 1.0f;
		RadarBodyColor.B = 1.0f;
		RadarBodyColor.A = 0.7f;
		RadarBody->SetColorAndOpacity(RadarBodyColor);
	}
}

UUserWidget* UWARadarWidget::AddEnemyMarker()
{
	APlayerController* PlayerController = GetOwningPlayer();
	UWARadarEnemyMarkerWidget* RadarEnemyMarkerWidget = CreateWidget<UWARadarEnemyMarkerWidget>(PlayerController, RadarEnemyMarkerWidgetClass);
	if (RadarEnemyMarkerWidget)
	{
		EnemyMarkerWidgets.Add(RadarEnemyMarkerWidget);

		UOverlaySlot* OverlaySlot = RadarOverlay->AddChildToOverlay(RadarEnemyMarkerWidget);
		OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
		OverlaySlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 7.0f));

		return RadarEnemyMarkerWidget;
	}
	else
	{
		return nullptr;
	}
}