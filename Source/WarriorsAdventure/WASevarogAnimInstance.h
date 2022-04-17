// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "WAAnimInstance.h"
#include "WASevarogAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWASevarogAnimInstance : public UWAAnimInstance
{
	GENERATED_BODY()
	
public:
	UWASevarogAnimInstance();

	virtual void PlayBasicAttackMontage(int32 CurrentAttackCount) override;
	virtual void PlayBackstepMontage() override;
	virtual void PlayDiseaseBallMontage();
	virtual void PlayDiseaseRainMontage();

private:
	UFUNCTION()
	void AnimNotify_FireDiseaseBall();

	UFUNCTION()
	void AnimNotify_SpawnDiseaseRain();
	UFUNCTION()
	void AnimNotify_FireDiseaseRain();

private:
	// Attack Montages.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_BasicAttack_A;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_BasicAttack_B;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_BasicAttack_C;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_Backstep;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_Disease_Ball;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_Disease_Rain;
};
