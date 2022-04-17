// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "UObject/NoExportTypes.h"
#include "CharacterSkill.generated.h"

UENUM()
enum class ESkillType
{
	Normal,
	Combo,
	Holding,
	Area
};

UENUM()
enum class ESkillRangeType
{
	Directional,
	Radial,
	Sector
};

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UCharacterSkill : public UObject
{
	GENERATED_BODY()
	
public:
	void Cast();

	ESkillType GetType();
	ESkillRangeType GetRangeType();
	float GetDamageMultiplier();
	float GetRange();
	float GetCapsuleRadius();
	float GetSectorAngle();

	void SetType(ESkillType NewType);
	void SetRangeType(ESkillRangeType NewRangeType);
	void SetDamageMultiplier(float NewDamageMultiplier);
	void SetRange(float NewRange);
	void SetCapsuleRadius(float NewCapsuleRadius);
	void SetSectorAngle(float NewSectorAngle);
	
	
private:
	FName SkillName;

	ESkillType SkillType;
	ESkillRangeType SkillRangeType;
	float SkillDamageMultiplier;
	float SkillRange;
	float SkillCapsuleRadius;
	float SkillSectorAngle;
};
