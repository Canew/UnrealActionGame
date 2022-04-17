// Fill out your copyright notice in the Description page of Project Settings.


#include "WACharacterWidget.h"
#include "Components/CharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UWACharacterWidget::BindCharacterStat(UCharacterStatComponent* NewCharacterStat)
{
	if (NewCharacterStat == nullptr)
	{
		return;
	}

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddDynamic(this, &UWACharacterWidget::UpdateHPWidget);
	UpdateHPWidget();
}

void UWACharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	UpdateHPWidget();
}

void UWACharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat)
	{
		if (HPBar)
		{
			HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
			if (CurrentCharacterStat->GetHPRatio() >= 1.0f)
			{
				SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}