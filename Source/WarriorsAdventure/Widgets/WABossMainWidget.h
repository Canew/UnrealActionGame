// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WABossMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWABossMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUserWidget* GetBossHpWidget();

private:
	UPROPERTY(Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	class UUserWidget* BossHpWidget;
};
