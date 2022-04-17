// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WARadarDirectionWidget.h"
#include "WARadarMarkerWidget.h"
#include "WARadarEnemyMarkerWidget.h"
#include "WARadarLandmarkWidget.h"
#include "WARadarWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWARadarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UWARadarWidget(const FObjectInitializer& ObjectInitializer);

	class UWidgetAnimation* GetBlendOutAnim();
	UUserWidget* AddDirection(FDirectionInfo Info);
	UUserWidget* AddMarker(EMarkerType Type);
	UUserWidget* AddEnemyMarker();
	UUserWidget* AddLandmark(UTexture2D* Texture);
	void HighlightRadar(bool Highlight);

	TArray<class UWARadarDirectionWidget*> GetDirectionWidgets() { return DirectionWidgets; }
	TArray<class UWARadarMarkerWidget*> GetMarkerWidgets() { return MarkerWidgets; }
	TArray<class UWARadarEnemyMarkerWidget*> GetEnemyMarkerWidgets() { return EnemyMarkerWidgets; }
	TArray<class UWARadarLandmarkWidget*> GetLandmarkWidgets() { return LandmarkWidgets; }
	
private:
	UPROPERTY(Meta = (BindWidgetAnim), Meta = (AllowPrivateAccess = true))
	class UWidgetAnimation* BlendOut;

	UPROPERTY(Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	class UOverlay* RadarOverlay;

	UPROPERTY(Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	class UImage* RadarBody;

	TArray<class UWARadarDirectionWidget*> DirectionWidgets;
	TArray<class UWARadarMarkerWidget*> MarkerWidgets;
	TArray<class UWARadarEnemyMarkerWidget*> EnemyMarkerWidgets;
	TArray<class UWARadarLandmarkWidget*> LandmarkWidgets;

	TSubclassOf<class UWARadarDirectionWidget> RadarDirectionWidgetClass;
	TSubclassOf<class UWARadarMarkerWidget> RadarMarkerWidgetClass;
	TSubclassOf<class UWARadarEnemyMarkerWidget> RadarEnemyMarkerWidgetClass;
	TSubclassOf<class UWARadarLandmarkWidget> RadarLandmarkWidgetClass;
};
