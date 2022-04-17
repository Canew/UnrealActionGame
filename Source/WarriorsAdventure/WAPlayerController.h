// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "WAPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API AWAPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	AWAPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// UI methods.
	 
	class UWAStatHudUserWidget* GetStatHudWidget() { return StatHudWidget; }
	void DisplayInteractWidget(bool Display);
	void AddBossWidget(class AWACharacter* InBossCharacter);
	void RemoveBossWidget();

	// Generic Team

	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	// Stat UI

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UWAStatHudUserWidget> StatHudWidgetClass;
	UPROPERTY()
	class UWAStatHudUserWidget* StatHudWidget;

	// Interact UI

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> InteractWidgetClass;
	UPROPERTY()
	class UUserWidget* InteractWidget;

	// Minimap UI

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> MapWidgetClass;
	UPROPERTY()
	class UUserWidget* MapWidget;

	// Boss UI

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> BossMainWidgetClass;
	UPROPERTY()
	class UWABossMainWidget* BossMainWidget;

	// Generic Team

	FGenericTeamId TeamId;
};
