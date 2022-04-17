// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WARadarDirectionWidget.generated.h"

USTRUCT(BlueprintType)
struct FDirectionInfo
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WorldRotation;
};

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWARadarDirectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;

	FDirectionInfo GetDirectionInfo() { return DirectionInfo;; }
	void SetDirectionInfo(FDirectionInfo Info) { DirectionInfo = Info; }

private:
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* DirectionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	struct FDirectionInfo DirectionInfo;
};
