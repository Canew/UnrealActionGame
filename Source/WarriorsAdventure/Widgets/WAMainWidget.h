// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WAMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWAMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	class UWARadarWidget* GetRadarWidget();
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(Meta = (BindWidget))
	class UWARadarWidget* RadarWidget;

};
