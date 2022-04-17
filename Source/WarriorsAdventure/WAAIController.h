// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "WAAIController.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API AWAAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AWAAIController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void PerceptionUpdated(const TArray<AActor*>& Actors);

	UFUNCTION()
	virtual void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	virtual void SetFocus(AActor* NewFocus, EAIFocusPriority::Type InPriority = EAIFocusPriority::Gameplay) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName EnemyCharacter = TEXT("EnemyCharacter");
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName HasLineOfSight = TEXT("HasLineOfSight");
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName PatrolLocation = TEXT("PatrolLocation");
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName RandomPatternValue = TEXT("RandomPatternValue");

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BT_MeleeBasicMinion;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UBlackboardData* BB_MeleeBasicMinion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AISightRadius = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AISightAge = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AILoseSightRadius = AISightRadius + 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AIFieldOfView = 180.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FTimerHandle LineOfSightTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float LineOfSightTimer = 15.0f;


};
