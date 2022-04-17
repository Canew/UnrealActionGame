// Fill out your copyright notice in the Description page of Project Settings.


#include "WAPlayerController.h"
#include "WACharacter.h"
#include "WAStatHudUserWidget.h"
#include "Widgets/WABossHpWidget.h"
#include "Widgets/WABossMainWidget.h"

AWAPlayerController::AWAPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_STATHUD(TEXT("WidgetBlueprint'/Game/UI/UI_HUD.UI_HUD_C'"));
	if (UI_STATHUD.Succeeded())
	{
		StatHudWidgetClass = UI_STATHUD.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_INTERACT(TEXT("WidgetBlueprint'/Game/UI/UI_Interact.UI_Interact_C'"));
	if (UI_INTERACT.Succeeded())
	{
		InteractWidgetClass = UI_INTERACT.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_MAP(TEXT("WidgetBlueprint'/Game/Minimap/Widgets/UI_MapMain.UI_MapMain_C'"));
	if (UI_MAP.Succeeded())
	{
		MapWidgetClass = UI_MAP.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_BOSS_MAIN(TEXT("WidgetBlueprint'/Game/UI/UI_Boss/UI_BossMainWidget.UI_BossMainWidget_C'"));
	if (UI_BOSS_MAIN.Succeeded())
	{
		BossMainWidgetClass = UI_BOSS_MAIN.Class;
	}

	TeamId = FGenericTeamId(1);
}

void AWAPlayerController::BeginPlay()
{
	Super::BeginPlay();

	StatHudWidget = CreateWidget<UWAStatHudUserWidget>(this, StatHudWidgetClass);
	if (StatHudWidget != nullptr)
	{
		StatHudWidget->AddToViewport();
	}

	InteractWidget = CreateWidget<UUserWidget>(this, InteractWidgetClass);
	MapWidget = CreateWidget<UUserWidget>(this, MapWidgetClass);
	MapWidget->AddToViewport();
}

void AWAPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

}

void AWAPlayerController::DisplayInteractWidget(bool Display)
{
	if (Display)
	{
		if (InteractWidget->IsInViewport())
		{
			return;
		}
		else
		{
			InteractWidget->AddToViewport();
		}
	}
	else
	{
		InteractWidget->RemoveFromParent();
	}
}

void AWAPlayerController::AddBossWidget(AWACharacter* InBossCharacter)
{
	if (InBossCharacter == nullptr)
	{
		return;
	}

	if (BossMainWidget && BossMainWidget->IsInViewport())
	{
		UE_LOG(LogTemp, Log, TEXT("AddBossWidget() if(BossMainWidget && BossMainWidget->IsInViewport())"));

		return;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AddBossWidget() else(BossMainWidget && BossMainWidget->IsInViewport())"));
		BossMainWidget = CreateWidget<UWABossMainWidget>(this, BossMainWidgetClass);
		if (BossMainWidget)
		{
			UWABossHpWidget* BossHpWidget = Cast<UWABossHpWidget>(BossMainWidget->GetBossHpWidget());
			if (BossHpWidget)
			{
				BossHpWidget->BindBossCharacter(InBossCharacter);
				BossMainWidget->AddToViewport();
			}
		}
	}
}

void AWAPlayerController::RemoveBossWidget()
{
	if (BossMainWidget)
	{
		BossMainWidget->RemoveFromParent();
		BossMainWidget = nullptr;
	}
}

FGenericTeamId AWAPlayerController::GetGenericTeamId() const
{
	return TeamId;
}

ETeamAttitude::Type AWAPlayerController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			FGenericTeamId OtherTeamId = TeamAgent->GetGenericTeamId();
			if (GetGenericTeamId() == OtherTeamId)
			{
				ETeamAttitude::Friendly;
			}
			else
			{
				ETeamAttitude::Hostile;
			}
		}
	}

	return ETeamAttitude::Neutral;
}
