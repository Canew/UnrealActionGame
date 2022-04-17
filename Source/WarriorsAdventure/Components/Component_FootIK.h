// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "Components/ActorComponent.h"
#include "Component_FootIK.generated.h"

// Line Trace info struct
typedef struct IKTraceInfo
{
	float Offset;
	FVector ImpactLocation;
};

// IK Anim Instance Value struct
USTRUCT(Atomic, BlueprintType)
struct FIKAnimValue
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectorLocation_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectorLocation_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator FootRotation_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator FootRotation_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HipOffset;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARRIORSADVENTURE_API UComponent_FootIK : public UActorComponent
{
	GENERATED_BODY()

private:
	// Owner Characters capsule height
	float IKCapsuleHalkHeight;

	// IK Anim Instance Value struct
	FIKAnimValue IKAnimValuse;

	// IK Active state
	bool bActive = false;

public:
	// Default Values

	// Left foot bone name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
	FName IKSocketName_LeftFoot;
	// Right foot bone name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
	FName IKSocketName_RightFoot;

	// Distance between the floor and the foot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
	float IKAdjustOffset;

	// Max Linetrace distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
	float IKTraceDistance;

	// Hip(pelvis) move speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
	float IKHipsInterpSpeed;

	// Leg joing move speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
	float IKFeetInterpSpeed;

	// Enable debug mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Debug")
	bool bDebug;

public:
	UComponent_FootIK();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Set foot bone name from skeletalmesh
	void SetIKSocketName(FString sSocName_LeftFoot, FString sSocName_RightFoot);

	// Reset IK State
	void IKResetVars();

	// Set IK Active state
	void SetIKActive(bool bActive);

	// Set IK Debug state
	void SetIKDebug(bool bActive);

private:
	// IK process update
	void IKUpdate(float fDeltaTime);

	// IK Debug mode update
	void IKDebug();

	// process IK Foot,leg Z Offset
	void IKUpdateFootOffset(float fDeltaTime, float fTargetValue, float* fEffectorValue, float fInterpSpeed);

	// Process IK Foot rotation from ground normal
	void IKUpdateFootRotation(float fDeltaTime, FRotator pTargetValue, FRotator* pFootRotatorValue, float fInterpSpeed);

	// Process IK characters capsule component Z Offset
	void IKUpdateCapsuleHalfHeight(float fDeltaTime, float fHipsShifts, bool bResetDefault);

	// Get foot line trace info
	IKTraceInfo IKFootTrace(float fTraceDistance, FName sSocket);

	// Get ground normal
	FRotator NormalToRotator(FVector pVector);

	// Check owner character is moving
	bool IsMoving();

public:
	FORCEINLINE FIKAnimValue GetIKAnimValue() { return IKAnimValuse; }
	FORCEINLINE bool GetIKDebugState() { return bDebug; }
};
