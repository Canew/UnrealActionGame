// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "Animation/AnimInstance.h"
#include "WAAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSpecialAttackHitCheckDelegate);

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWAAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void SetInputMoveDirectionX(float NewAxisValue);
	virtual void SetInputMoveDirectionY(float NewAxisValue);

	virtual void PlayBasicAttackMontage(int32 CurrentAttackCount) {}
	virtual void PlayChargedBasicAttackMontage() {}
	virtual void PlayDashAttackMontage() {}
	virtual void PlayJumpAttackMontage() {}
	virtual void PlaySpecialAttackMontage() {}

	virtual void PlayUltimateMontage() {};
	virtual void PlayHitstunMontage() {}
	virtual void PlayBackstepMontage() {}

	virtual void SetDead(bool Dead) { bDead = Dead; }
	virtual void SetDashing(bool Dashing) { bDashing = Dashing; }

protected:
	virtual void GetAimOffsetValue(AActor* Center, AActor* Target);

private:
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_SpecialAttackHitCheck();

public:
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnSpecialAttackHitCheckDelegate OnSpecialAttackHitCheck;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	ECharacterAction CurrentCharacterAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnDirection = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float InputMoveDirectionX = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float InputMoveDirectionY = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bDashing = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bInAir = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bWallJumped = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bFixedRotationToController = false;

	UPROPERTY(BlueprintReadOnly, Category = AimOffset, Meta = (AllowPrivateAccess = true))
	float AimOffsetYaw = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = AimOffset, Meta = (AllowPrivateAccess = true))
	float AimOffsetPitch = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bDead = false;
};
