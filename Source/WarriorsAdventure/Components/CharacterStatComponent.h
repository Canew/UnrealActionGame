// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHPChangedDelegate);

USTRUCT(BlueprintType)
struct FCharacterStat
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackHorizontalAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Armor;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARRIORSADVENTURE_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetCurrentHP(float NewCurrentHP) { CharacterStat.CurrentHP = NewCurrentHP; OnHPChanged.Broadcast(); if (CharacterStat.CurrentHP < KINDA_SMALL_NUMBER) OnHPIsZero.Broadcast(); }
	UFUNCTION(BlueprintCallable)
	void SetMaxHP(float NewMaxHP) { CharacterStat.MaxHP = NewMaxHP; }
	UFUNCTION(BlueprintCallable)
	void SetDamage(float NewDamage) { CharacterStat.Damage = NewDamage; }
	UFUNCTION(BlueprintCallable)
	void SetAttackRange(float NewAttackRange) { CharacterStat.AttackRange = NewAttackRange; }
	UFUNCTION(BlueprintCallable)
	void SetAttackHorizontalAngle(float NewAttackHorizontalAngle) { CharacterStat.AttackHorizontalAngle = NewAttackHorizontalAngle; }
	UFUNCTION(BlueprintCallable)
	void SetAttackHalfHeight(float NewAttackHalfHeight) { CharacterStat.AttackHalfHeight = NewAttackHalfHeight; }
	UFUNCTION(BlueprintCallable)
	void SetArmor(float NewArmor) { CharacterStat.Armor = NewArmor; }

	UFUNCTION(BlueprintCallable)
	float GetCurrentHP() { return CharacterStat.CurrentHP; }
	UFUNCTION(BlueprintCallable)
	float GetMaxHP() { return CharacterStat.MaxHP; }
	UFUNCTION(BlueprintCallable)
	float GetHPRatio() { return CharacterStat.CurrentHP / CharacterStat.MaxHP; }
	UFUNCTION(BlueprintCallable)
	float GetDamage() { return CharacterStat.Damage; }
	UFUNCTION(BlueprintCallable)
	float GetAttackRange() { return CharacterStat.AttackRange; }
	UFUNCTION(BlueprintCallable)
	float GetAttackHorizontalAngle() { return CharacterStat.AttackHorizontalAngle; }
	UFUNCTION(BlueprintCallable)
	float GetAttackHalfHeight() { return CharacterStat.AttackHalfHeight; }
	UFUNCTION(BlueprintCallable)
	float GetArmor() { return CharacterStat.Armor; }

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FOnHPIsZeroDelegate OnHPIsZero;
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FOnHPChangedDelegate OnHPChanged;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FCharacterStat CharacterStat;
};
