// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "Blueprint/UserWidget.h"
#include "WACharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWACharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UCharacterStatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void UpdateHPWidget();

private:
	class UCharacterStatComponent* CurrentCharacterStat;

	UPROPERTY()
	class UProgressBar* HPBar;
};
