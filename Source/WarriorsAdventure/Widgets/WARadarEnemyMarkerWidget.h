// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WARadarEnemyMarkerWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWARadarEnemyMarkerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UWARadarEnemyMarkerWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	void UpdateDistance(int32 Distance);

private:
	UPROPERTY(Meta = (BindWidget))
	class UImage* EnemyMarkerIcon;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* DistanceText;

	UTexture2D* EnemyMarkerTexture;

	int32 MaxDisplayedDistance = 999;
};
