// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WAAnimInstance.h"
#include "Component_FootIK.h"
#include "WAGothicKnightAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWAGothicKnightAnimInstance : public UWAAnimInstance
{
	GENERATED_BODY()

public:
	UWAGothicKnightAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Play Attack Montages.
	virtual void PlayBasicAttackMontage(int32 CurrentAttackCount) override;
	virtual void PlayChargedBasicAttackMontage() override;
	virtual void PlayDashAttackMontage() override;
	virtual void PlayJumpAttackMontage() override;
	virtual void PlaySpecialAttackMontage() override;

	virtual void PlayUltimateMontage();

	// Play Guard Montages.
	virtual void PlayGuardOnMontage();
	virtual void PlayGuardOffMontage();
	virtual void PlayGuardBlockMontage();
	virtual void PlayGuardParryMontage();

	// Play Dodge Montages.
	virtual void PlayEvadeFrontMontage();
	virtual void PlayEvadeBackMontage();
	virtual void PlayEvadeLeftMontage();
	virtual void PlayEvadeRightMontage();

	// Play Hit React Montage.
	virtual void PlayHitstunMontage();

protected:
	// Foot IK properties.
	UPROPERTY()
	class UComponent_FootIK* IK_Foot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IKFoot Value", meta = (AllowPrivateAccess = "true"))
	FIKAnimValue IKAnimValue;

	// Attack montages.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_BasicAttack_A;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_BasicAttack_B;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_BasicAttack_C;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_BasicAttack_D;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_ChargedBasicAttack;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_DashAttack;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_JumpAttack;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_SpecialAttack;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_Ultimate;

	// Guard montages.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Guard, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_Guard;

	// Evade montages.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
	bool bEvading = false;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_Evade;

	// Hit react montages.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_Hitstun;
	
};
