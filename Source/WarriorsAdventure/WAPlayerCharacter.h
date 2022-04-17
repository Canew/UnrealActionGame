// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "WACharacter.h"
#include "WAPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API AWAPlayerCharacter : public AWACharacter
{
	GENERATED_BODY()
	
public:
	AWAPlayerCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input methods.
	virtual void UpDown(float NewAxisValue) override;
	virtual void LeftRight(float NewAxisValue) override;
	virtual void Turn(float NewAxisValue) override;

	// Interact methods.
	virtual void Interact();

	// Target Lock methods.
	virtual void TargetLock();
	virtual void TargetUnlock();
	virtual bool IsTargetLocking() { return bTargetLock; }
	virtual class AWACharacter* GetLockedTarget() { if (LockedTarget.IsValid()) return LockedTarget.Get(); else return nullptr; }
	
	// UI methods
	virtual void ToggleUI();
	virtual void ToggleInventory();

	// Inventory methods.

	class AWAInventory* GetInventory();

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

protected:
	UPROPERTY(VisibleAnywhere, Category = Interact, meta = (AllowPrivateAccess = true))
	class UWAInteractComponent* InteractComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Radar", meta = (AllowPrivateAccess = true))
	class UWARadarComponent* CharacterRadar;

	// Inventory

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = true))
	class AWAInventory* CharacterInventory;

	// Minimap

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map", meta = (AllowPrivateAccess = true))
	class UMapViewComponent* MapViewComponent;

	// Target Lock properties.
	bool bTargetLock = false;
	TWeakObjectPtr<AActor> TargetSphere;
	TWeakObjectPtr<AWACharacter> LockedTarget;
	int32 TargetLockRange = 3000.0f;
};

