// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "WAEnemyCharacter.h"
#include "WAMeleeBasicMinion.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API AWAMeleeBasicMinion : public AWAEnemyCharacter
{
	GENERATED_BODY()
	
public:
	AWAMeleeBasicMinion();

	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};
