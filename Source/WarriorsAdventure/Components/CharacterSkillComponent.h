// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "Components/ActorComponent.h"
#include "CharacterSkill.h"
#include "CharacterSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARRIORSADVENTURE_API UCharacterSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterSkillComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetCharacterSkillData(FString CharacterSkillDataPath);

	virtual class UCharacterSkill* FindSkill(FName SkillName);
	virtual void AddSkill(FName NewSkillName, class UCharacterSkill* NewSkill);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TMap<FName, class UCharacterSkill*> CharacterSkills;
};
