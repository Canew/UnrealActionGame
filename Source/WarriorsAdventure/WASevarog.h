// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "WAEnemyCharacter.h"
#include "WASevarog.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireDiseaseRainDelegate);

UCLASS()
class WARRIORSADVENTURE_API AWASevarog : public AWAEnemyCharacter
{
	GENERATED_BODY()

public:
	AWASevarog();

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	// Skill Methods.
	virtual void PlayDiseaseBall();
	virtual void FireDiseaseBall();

	virtual void PlayDiseaseRain();
	virtual void SpawnDiseaseRain();
	virtual void FireDiseaseRain();

    // AI Methods.
	virtual void ExecuteMeleePattern();
	virtual void ExecuteRangePattern();

public:
	UPROPERTY(BlueprintReadOnly)
	FOnFireDiseaseRainDelegate OnFireDiseaseRain;

private:
	UPROPERTY(BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	class UWASevarogAnimInstance* SevarogAnimInstance;

	TSubclassOf<AActor> DiseaseBallClass;
	TSubclassOf<AActor> DiseaseRainClass;
};
