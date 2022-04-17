// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "WACharacter.h"
#include "WASwordman.generated.h"

UCLASS()
class WARRIORSADVENTURE_API AWASwordman : public AWACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWASwordman();

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

	// Guard Methods.
	virtual void GuardOn();
	virtual void GuardOff();

	// Evade Methods.
	virtual void EvadeFront();
	virtual void EvadeBack();
	virtual void EvadeLeft();
	virtual void EvadeRight();

protected:
	virtual void SpecialAttackCheck();

	UFUNCTION()
	virtual void SpecialAttackBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

public:
	// Foot IK
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK_Foot", meta = (AllowPrivateAccess = "true"))
	class UComponent_FootIK* IK_Foot;

private:
	// Guard properties.
	bool bGuardBlocking = false;
	FVector GuardBlockingPushDirection;
	float GuardBlockingPushVelocity = 0.0f;
};
