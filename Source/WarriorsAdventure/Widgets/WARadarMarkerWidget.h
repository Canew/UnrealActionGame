// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WARadarMarkerWidget.generated.h"

UENUM()
enum class EMarkerType
{
	MainQuest,
	SideQuest
};

USTRUCT(BlueprintType)
struct FMarkerInfo
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMarkerType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
};

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWARadarMarkerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UWARadarMarkerWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	void UpdateDistance(int32 Distance);

	void SetMarkerType(EMarkerType Type) { MarkerType = Type; }

private:
	UPROPERTY(Meta = (BindWidget))
	class UImage* MarkerIcon;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* DistanceText;

	UTexture2D* MainQuestIcon;
	UTexture2D* SideQuestIcon;

	EMarkerType MarkerType;
	int32 MaxDisplayedDistance = 999;
};
