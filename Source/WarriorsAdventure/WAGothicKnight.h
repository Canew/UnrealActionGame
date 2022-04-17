// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "WAPlayerCharacter.h"
#include "WAGothicKnight.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API AWAGothicKnight : public AWAPlayerCharacter
{
	GENERATED_BODY()
	
public:
	AWAGothicKnight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Skill Methods.
	virtual void Ultimate() override;

	// Guard Methods.
	virtual void GuardOn();
	virtual void GuardOff();

	// Evade Methods.
	virtual void EvadeFront();
	virtual void EvadeBack();
	virtual void EvadeLeft();
	virtual void EvadeRight();

	// Delegate
	virtual void OnRightMouseButtonUp();

private:
	virtual void SpecialAttackHitCheck();

public:
	// Foot IK
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK_Foot", meta = (AllowPrivateAccess = true))
	class UComponent_FootIK* IK_Foot;

private:
	// Weapon
	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* Weapon;

	// Guard properties.
	bool bGuardBlocking = false;
	FVector GuardBlockingPushDirection;
	float GuardBlockingPushVelocity = 0.0f;

	// Special Attack properties.
	UNiagaraSystem* SpecialAttackImpactNiagara;
};
