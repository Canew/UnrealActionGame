// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WAAnimInstance.h"
#include "WAMeleeBasicMinionAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWAMeleeBasicMinionAnimInstance : public UWAAnimInstance
{
	GENERATED_BODY()
	
public:
	UWAMeleeBasicMinionAnimInstance();

	virtual void PlayBasicAttackMontage(int32 CurrentAttackCount) override;

	virtual void PlayHitstunMontage() override;
	
protected:
	// Attack montages.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_BasicAttack_A;

	// Hitstun montages.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Montage_Hitstun;
};
