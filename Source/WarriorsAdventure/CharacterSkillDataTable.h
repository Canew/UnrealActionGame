// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "CharacterSkill.h"
#include "CharacterSkillDataTable.generated.h"

USTRUCT(BlueprintType)
struct FWACharacterSkillData : public FTableRowBase
{
	GENERATED_BODY()

	public:
	FWACharacterSkillData() : Type(ESkillType::Normal), RangeType(ESkillRangeType::Directional),
		DamageMultiplier(1.0f), Range(0.0f), CapsuleRadius(0.0f), SectorAngle(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	ESkillType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	ESkillRangeType RangeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float DamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float CapsuleRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SectorAngle;
};

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UCharacterSkillDataTable : public UObject
{
	GENERATED_BODY()
	
};
