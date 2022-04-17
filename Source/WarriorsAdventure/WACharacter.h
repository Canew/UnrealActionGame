// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "GameFramework/Character.h"
#include "WACharacter.generated.h"

UCLASS()
class WARRIORSADVENTURE_API AWACharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWACharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	virtual ECharacterAction GetCharacterAction() { return CurrentCharacterAction; }
	virtual void SetCharacterAction(ECharacterAction NewCharacterAction);

	// Attack methods.
	virtual void Attack();
	virtual void AttackReleased();

	virtual void BasicAttack(int32 NewBasicAttackCombo);
	virtual void BasicAttackEnd();
	virtual void BasicAttackCombo();
	virtual bool IsNextBasicAttackInputOn() { return bNextBasicAttackInputOn; }
	virtual void SetNextBasicAttackInputOn(bool NextBasicAttackInputOn) { bNextBasicAttackInputOn = NextBasicAttackInputOn; }
	virtual int32 GetCurrentBasicAttackCombo() { return CurrentBasicAttackCombo; }
	virtual int32 GetMaxBasicAttackCombo() { return MaxBasicAttackCombo; }

	virtual void ChargedBasicAttack();

	virtual void SpecialAttack();

	// Skill methods.
	virtual void Ultimate();	

	virtual void Backstep();
	virtual bool CanWallJump() { return bCanWallJump; }

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// AI Methods.
	virtual void ExecuteMeleePattern() {};
	virtual void ExecuteRangePattern() {};

protected:

	// Move methods.
	virtual void Move();
	virtual void Rotate();
	virtual void UpDown(float NewAxisValue);
	virtual void LeftRight(float NewAxisValue);
	virtual void Turn(float NewAxisValue);
	virtual void LookUp(float NewAxisValue);

	virtual void Dash();
	virtual void DashEnd();
	virtual void Jump() override;

	// Attack methods.
	virtual void AttackHitCheck();
	virtual bool IsInAngle(TWeakObjectPtr<AWACharacter> Center, TWeakObjectPtr<AActor> Target, float AttackAngle);

	// Be Hit methods.
	virtual void Hitstun();
	UFUNCTION()
	virtual void Dead();

	bool IsTeamTowards(const AActor& Other);

public:
	UPROPERTY(BlueprintReadOnly, Category = Anim)
	class UWAAnimInstance* AnimInstance;

	UPROPERTY(BlueprintReadOnly, Category = Audio)
	class UAudioComponent* CharacterAudio;

	UPROPERTY(BlueprintReadOnly, Category = Audio)
	class USoundBase* BeHitSoundCue;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat)
	class UCharacterStatComponent* CharacterStat;

	TSubclassOf<class UCameraShake> AttackCameraShake;

	UParticleSystem* HitImpactParticle;
	class UNiagaraSystem* HitImpactNiagara;

protected:
	UPROPERTY(VisibleAnywhere, Category = Action)
	ECharacterAction CurrentCharacterAction = ECharacterAction::Idle;

	// Move properties.
	FVector MoveDirection;
	bool bCanWallJump = true;
	float RunSpeed = 600.0f;

	bool bDashing = false;
	float DashSpeed = 800.0f;

	// Attack properties.
	bool bNextBasicAttackInputOn = false;
	int32 CurrentBasicAttackCombo = 0;
	int32 MaxBasicAttackCombo = 1;

	FTimerHandle ChargedBasicAttackTimerHandle;
	float ChargedBasicAttackTime = 0.4f;

	// Guard properties.
	bool bCanParry = false;
	float ParryableTime = 0.4f;

	// Hit react properties.
	FVector HitstunPushVelocity;

	// AI Properties.
	int32 MaxMeleePatternKind = 0;
	int32 MaxRangePatternKind = 0;
};
