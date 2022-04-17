// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WABossHpWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWABossHpWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindBossCharacter(class AWACharacter* InCharacter);
	UFUNCTION()
	void OnBossHpChanged();

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	class UTextBlock* BossNameText;

	UPROPERTY(Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	class UProgressBar* BossHpBar;

	UPROPERTY()
	class AWACharacter* BossCharacter;

	FTimerHandle RemoveWidgetTimerHandle;
	float RemoveWidgetTime = 2.0f;
};
