// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Component_CharacterStat.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARRIORSADVENTURE_API UComponent_CharacterStat : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponent_CharacterStat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetCurrentHP(float NewCurrentHP) { CurrentHP = NewCurrentHP; }
	void SetMaxHP(float NewMaxHP) { MaxHP = NewMaxHP; }
	void SetDamage(float NewDamage) { Damage = NewDamage; }
	void SetArmor(float NewArmor) { Armor = NewArmor; }
	float GetCurrentHP() { return CurrentHP; }
	float GetMaxHP() { return MaxHP; }
	float GetDamage() { return Damage; }
	float GetArmor() { return Armor; }

private:
	float CurrentHP = 0.0f;
	float MaxHP = 0.0f;
	float Damage = 0.0f;
	float Armor = 0.0f;
};
