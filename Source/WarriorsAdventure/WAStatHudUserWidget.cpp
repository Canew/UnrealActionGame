// Fill out your copyright notice in the Description page of Project Settings.


#include "WAStatHudUserWidget.h"
#include "Components/CharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UWAStatHudUserWidget::BindCharacterStat(class UCharacterStatComponent* CharacterStat)
{
	if (CharacterStat == nullptr)
		return;

	CurrentCharacterStat = CharacterStat;
	CurrentCharacterStat->OnHPChanged.AddDynamic(this, &UWAStatHudUserWidget::UpdateCharacterStat);
}

void UWAStatHudUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HealthBar")));
	if (HealthBar == nullptr)
		return;

	EnergyBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_EnergyBar")));
	if (EnergyBar == nullptr)
		return;
}

void UWAStatHudUserWidget::UpdateCharacterStat()
{
	if (CurrentCharacterStat.IsValid())
	{
		HealthBar->SetPercent(CurrentCharacterStat->GetHPRatio());
	}
}