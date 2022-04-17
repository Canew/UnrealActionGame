// Fill out your copyright notice in the Description page of Project Settings.


#include "WABossHpWidget.h"
#include "WACharacter.h"
#include "WAPlayerController.h"
#include "WAAIController.h"
#include "Components/CharacterStatComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UWABossHpWidget::BindBossCharacter(class AWACharacter* InBossCharacter)
{
	if (InBossCharacter)
	{
		BossCharacter = InBossCharacter;
		BossCharacter->CharacterStat->OnHPChanged.AddDynamic(this, &UWABossHpWidget::OnBossHpChanged);
		OnBossHpChanged();
	}
}

void UWABossHpWidget::OnBossHpChanged()
{
	if (BossCharacter)
	{
		BossHpBar->SetPercent(BossCharacter->CharacterStat->GetHPRatio());
	}
}

void UWABossHpWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (BossCharacter == nullptr)
	{
		RemoveFromParent();
		return;
	}

	AAIController* AIController = Cast<AAIController>(BossCharacter->GetController());
	if (AIController && AIController->GetFocusActor() == nullptr)
	{
		Cast<AWAPlayerController>(GetOwningPlayer())->RemoveBossWidget();
		return;
	}
}
