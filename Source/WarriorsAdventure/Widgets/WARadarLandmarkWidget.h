// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WARadarLandmarkWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWARadarLandmarkWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;

	void SetLandmarkTexture(UTexture2D* Texture);

private:
	UPROPERTY(Meta = (BindWidget))
	class UImage* LandmarkIcon;
	class UTexture2D* LandmarkTexture;
};
