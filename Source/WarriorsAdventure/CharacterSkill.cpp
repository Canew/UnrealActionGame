// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkill.h"

void UCharacterSkill::Cast()
{

}

ESkillType UCharacterSkill::GetType()
{
	return SkillType;
}

ESkillRangeType UCharacterSkill::GetRangeType()
{
	return SkillRangeType;
}

float UCharacterSkill::GetDamageMultiplier()
{
	return SkillDamageMultiplier;
}

float UCharacterSkill::GetRange()
{
	return SkillRange;
}

float UCharacterSkill::GetCapsuleRadius()
{
	return SkillCapsuleRadius;
}

float UCharacterSkill::GetSectorAngle()
{
	return SkillSectorAngle;
}

void UCharacterSkill::SetType(ESkillType NewType)
{
	SkillType = NewType;
}

void UCharacterSkill::SetRangeType(ESkillRangeType NewRangeType)
{
	SkillRangeType = NewRangeType;
}

void UCharacterSkill::SetDamageMultiplier(float NewDamageMultiplier)
{
	SkillDamageMultiplier = NewDamageMultiplier;
}

void UCharacterSkill::SetRange(float NewRange)
{
	SkillRange = NewRange;
}

void UCharacterSkill::SetCapsuleRadius(float NewCapsuleRadius)
{
	SkillCapsuleRadius = NewCapsuleRadius;
}

void UCharacterSkill::SetSectorAngle(float NewSectorAngle)
{
	SkillSectorAngle = NewSectorAngle;
}
