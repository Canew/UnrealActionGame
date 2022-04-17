// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterSkillComponent.h"
#include "CharacterSkill.h"
#include "CharacterSkillDataTable.h"

// Sets default values for this component's properties
UCharacterSkillComponent::UCharacterSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterSkillComponent::SetCharacterSkillData(FString CharacterSkillDataPath)
{
	UDataTable* CharacterSkillDataTable;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CHARACTER_SKILL(*CharacterSkillDataPath);
	if (DT_CHARACTER_SKILL.Succeeded())
	{
		CharacterSkillDataTable = DT_CHARACTER_SKILL.Object;
	}
	else
	{
		return;
	}

	TArray<FName> RowNames;
	RowNames = CharacterSkillDataTable->GetRowNames();

	for (FName RowName : RowNames)
	{
		FWACharacterSkillData* CharacterSkillData = CharacterSkillDataTable->FindRow<FWACharacterSkillData>(RowName, TEXT(""));
		if (CharacterSkillData)
		{
			UCharacterSkill CharacterSkill;
			CharacterSkill.SetType(CharacterSkillData->Type);
			CharacterSkill.SetRangeType(CharacterSkillData->RangeType);
			CharacterSkill.SetDamageMultiplier(CharacterSkillData->DamageMultiplier);
			CharacterSkill.SetRange(CharacterSkillData->Range);
			CharacterSkill.SetCapsuleRadius(CharacterSkillData->CapsuleRadius);
			CharacterSkill.SetSectorAngle(CharacterSkillData->SectorAngle);
			
			AddSkill(RowName, &CharacterSkill);
		}
	}
}

UCharacterSkill* UCharacterSkillComponent::FindSkill(FName SkillName)
{
	return *(CharacterSkills.Find(SkillName));
}

void UCharacterSkillComponent::AddSkill(FName NewSkillName, UCharacterSkill* NewSkill)
{
	CharacterSkills.Add(NewSkillName, NewSkill);
}

