// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WAStatHudUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWAStatHudUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class UCharacterStatComponent* CharacterStat);

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void UpdateCharacterStat();

private:
	TWeakObjectPtr<class UCharacterStatComponent> CurrentCharacterStat;

	UPROPERTY()
	class UProgressBar* HealthBar;

	UPROPERTY()
	class UProgressBar* EnergyBar;
};
