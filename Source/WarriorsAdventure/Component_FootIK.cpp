
#include "Component_FootIK.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WACharacter.h"
#include "WAAnimInstance.h"

UComponent_FootIK::UComponent_FootIK()
{
	PrimaryComponentTick.bCanEverTick = true;

	IKFeetInterpSpeed = 22.0f;
	IKHipsInterpSpeed = 17.0f;
	IKTraceDistance = 55.0f;
	IKAdjustOffset = 5.0f;

	bDebug = false;
}

// Called when the game starts
void UComponent_FootIK::BeginPlay()
{
	Super::BeginPlay();

	AWACharacter* pCharacter = Cast<AWACharacter>(GetOwner());
	if (pCharacter == nullptr) return;

	IKCapsuleHalkHeight = pCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	SetIKActive(true);
}

void UComponent_FootIK::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UComponent_FootIK::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bActive == false)
	{
		return;
	}

	IKUpdate(DeltaTime);
	IKDebug();
}

void UComponent_FootIK::SetIKSocketName(FString sSocName_LeftFoot, FString sSocName_RightFoot)
{
	IKSocketName_LeftFoot = FName(*sSocName_LeftFoot);
	IKSocketName_RightFoot = FName(*sSocName_RightFoot);
}

void UComponent_FootIK::IKResetVars()
{
	// All reset the Animvalues
	IKUpdateFootOffset(GetWorld()->GetDeltaSeconds(), 0.0f, &IKAnimValuse.EffectorLocation_Right, IKFeetInterpSpeed);
	IKUpdateFootOffset(GetWorld()->GetDeltaSeconds(), 0.0f, &IKAnimValuse.EffectorLocation_Left, IKFeetInterpSpeed);

	IKUpdateFootRotation(GetWorld()->GetDeltaSeconds(), FRotator::ZeroRotator, &IKAnimValuse.FootRotation_Right, IKFeetInterpSpeed);
	IKUpdateFootRotation(GetWorld()->GetDeltaSeconds(), FRotator::ZeroRotator, &IKAnimValuse.FootRotation_Left, IKFeetInterpSpeed);

	IKUpdateFootOffset(GetWorld()->GetDeltaSeconds(), 0.0f, &IKAnimValuse.HipOffset, IKHipsInterpSpeed);
	IKUpdateCapsuleHalfHeight(GetWorld()->GetDeltaSeconds(), 0.0f, true);
}

void UComponent_FootIK::IKUpdate(float fDeltaTime)
{
	// Get Line Trace Info of ground from foot
	IKTraceInfo pTrace_Left = IKFootTrace(IKTraceDistance, IKSocketName_LeftFoot);
	IKTraceInfo pTrace_Right = IKFootTrace(IKTraceDistance, IKSocketName_RightFoot);

	if (IsMoving() == false)
	{
		// Update foot rotation from Impact normal
		IKUpdateFootRotation(fDeltaTime, NormalToRotator(pTrace_Left.ImpactLocation), &IKAnimValuse.FootRotation_Left, IKFeetInterpSpeed);
		IKUpdateFootRotation(fDeltaTime, NormalToRotator(pTrace_Right.ImpactLocation), &IKAnimValuse.FootRotation_Right, IKFeetInterpSpeed);
	}
	else
	{
		IKUpdateFootRotation(fDeltaTime, FRotator::ZeroRotator, &IKAnimValuse.FootRotation_Right, IKFeetInterpSpeed);
		IKUpdateFootRotation(fDeltaTime, FRotator::ZeroRotator, &IKAnimValuse.FootRotation_Left, IKFeetInterpSpeed);
	}


	// Update Hip Offset(Z)
	// Get Hip(Pelvis)Offset from foot offset and set CapsuleComponent height
	float fHipsOffset = UKismetMathLibrary::Min(pTrace_Left.Offset, pTrace_Right.Offset);
	if (fHipsOffset < 0.0f == false) fHipsOffset = 0.0f;
	IKUpdateFootOffset(fDeltaTime, fHipsOffset, &IKAnimValuse.HipOffset, IKHipsInterpSpeed);
	IKUpdateCapsuleHalfHeight(fDeltaTime, fHipsOffset, false);

	// Update effector locations of foot
	IKUpdateFootOffset(fDeltaTime, pTrace_Left.Offset - fHipsOffset, &IKAnimValuse.EffectorLocation_Left, IKFeetInterpSpeed);
	IKUpdateFootOffset(fDeltaTime, -1 * (pTrace_Right.Offset - fHipsOffset), &IKAnimValuse.EffectorLocation_Right, IKFeetInterpSpeed);

	// When foot is close to trace effector location disable IK
	// bool bLeftFootNearly = UKismetMathLibrary::NearlyEqual_FloatFloat(m_stIKAnimValuse.m_fEffectorLocation_Left, pTrace_Left.fOffset - fHipsOffset, 1.0f);
	// bool bRightFootNearly = UKismetMathLibrary::NearlyEqual_FloatFloat(m_stIKAnimValuse.m_fEffectorLocation_Right, pTrace_Right.fOffset - fHipsOffset, 1.0f);
	// if (bLeftFootNearly == true && bRightFootNearly == true) SetIKActive(false);

}

void UComponent_FootIK::IKDebug()
{
	// Debug AnimValues at Printstring
	if (bDebug == true)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Foot_L Rotation : " + IKAnimValuse.FootRotation_Left.ToString(), true, false, FLinearColor::Red, 0.0f);
		UKismetSystemLibrary::PrintString(GetWorld(), "Foot_R Rotation : " + IKAnimValuse.FootRotation_Right.ToString(), true, false, FLinearColor::Red, 0.0f);
		UKismetSystemLibrary::PrintString(GetWorld(), "Foot_L EffectorLocation(Z) : " + FString::SanitizeFloat(IKAnimValuse.EffectorLocation_Left), true, false, FLinearColor::Red, 0.0f);
		UKismetSystemLibrary::PrintString(GetWorld(), "Foot_R EffectorLocation(Z) : " + FString::SanitizeFloat(IKAnimValuse.EffectorLocation_Right), true, false, FLinearColor::Red, 0.0f);
		UKismetSystemLibrary::PrintString(GetWorld(), "HipOffset(Z) : " + FString::SanitizeFloat(IKAnimValuse.HipOffset), true, false, FLinearColor::Red, 0.0f);
	}

	// Debug the show Characters capsule collider
	AWACharacter* pCharacter = Cast<AWACharacter>(GetOwner());
	if (pCharacter != nullptr)
	{
		if (pCharacter->GetCapsuleComponent()->bHiddenInGame == bDebug)
		{
			pCharacter->GetCapsuleComponent()->bHiddenInGame = !bDebug;
		}
	}
}

void UComponent_FootIK::SetIKDebug(bool Active)
{
	bDebug = Active;
}


void UComponent_FootIK::IKUpdateFootOffset(float fDeltaTime, float fTargetValue, float* fEffectorValue, float fInterpSpeed)
{
	// Set foot Effectlocation value with FInterpTo
	float fInterpValue = UKismetMathLibrary::FInterpTo(*fEffectorValue, fTargetValue, fDeltaTime, fInterpSpeed);
	*fEffectorValue = fInterpValue;
}

void UComponent_FootIK::IKUpdateFootRotation(float fDeltaTime, FRotator pTargetValue, FRotator* pFootRotatorValue, float fInterpSpeed)
{
	// Set Foot Rotation value with FInterpTo
	FRotator pInterpRotator = UKismetMathLibrary::RInterpTo(*pFootRotatorValue, pTargetValue, fDeltaTime, fInterpSpeed);
	*pFootRotatorValue = pInterpRotator;
}

void UComponent_FootIK::IKUpdateCapsuleHalfHeight(float fDeltaTime, float fHipsShifts, bool bResetDefault)
{
	AWACharacter* pCharacter = Cast<AWACharacter>(GetOwner());
	if (pCharacter == nullptr)
	{
		return;
	}

	UCapsuleComponent* pCapsule = pCharacter->GetCapsuleComponent();
	if (pCapsule == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("IK : Capsule is NULL"));
		return;
	}

	// Get Half Height of capsule component
	float fCapsuleHalf = 0.0f;
	if (bResetDefault == true)
	{
		fCapsuleHalf = IKCapsuleHalkHeight;
	}
	else
	{
		float fHalfAbsSize = UKismetMathLibrary::Abs(fHipsShifts) * 0.5f;
		fCapsuleHalf = IKCapsuleHalkHeight - fHalfAbsSize;
	}

	// Set capsule component height with FInterpTo 
	float fScaledCapsuleHalfHeight = pCapsule->GetScaledCapsuleHalfHeight();
	float fInterpValue = UKismetMathLibrary::FInterpTo(fScaledCapsuleHalfHeight, fCapsuleHalf, fDeltaTime, IKHipsInterpSpeed);

	pCapsule->SetCapsuleHalfHeight(fInterpValue, true);
}

IKTraceInfo UComponent_FootIK::IKFootTrace(float fTraceDistance, FName sSocket)
{
	AWACharacter* pCharacter = Cast<AWACharacter>(GetOwner());
	if (pCharacter == nullptr)
	{
		return IKTraceInfo();
	}

	IKTraceInfo pTraceInfo;

	// Set Linetraces startpoint and end point
	FVector pSocketLocation = pCharacter->GetMesh()->GetSocketLocation(sSocket);
	FVector pLine_Start = FVector(pSocketLocation.X, pSocketLocation.Y, pCharacter->GetActorLocation().Z);
	FVector pLine_End = FVector(pSocketLocation.X, pSocketLocation.Y
		, (pCharacter->GetActorLocation().Z - IKCapsuleHalkHeight) - fTraceDistance);

	// Process Line Trace
	FHitResult pHitResult;
	TArray<AActor*> pIgnore;
	pIgnore.Add(GetOwner());

	EDrawDebugTrace::Type eDebug = EDrawDebugTrace::None;
	if (bDebug == true) eDebug = EDrawDebugTrace::ForOneFrame;

	bool bResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), pLine_Start, pLine_End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), true, pIgnore, eDebug, pHitResult, true);

	// Set ImpactNormal and Offset from HitResult
	pTraceInfo.ImpactLocation = pHitResult.ImpactNormal;
	if (pHitResult.IsValidBlockingHit() == true)
	{
		float fImpactLegth = (pHitResult.ImpactPoint - pHitResult.TraceEnd).Size();
		pTraceInfo.Offset = IKAdjustOffset + (fImpactLegth - fTraceDistance);
	}
	else
	{
		pTraceInfo.Offset = 0.0f;
	}

	return pTraceInfo;
}

bool UComponent_FootIK::IsMoving()
{
	AWACharacter* pCharacter = Cast<AWACharacter>(GetOwner());
	if (pCharacter == nullptr)
	{
		return false;
	}

	float Speed = pCharacter->GetVelocity().Size();
	if (Speed > 0.0f)
	{
		return true;
	}

	return false;
}

void UComponent_FootIK::SetIKActive(bool Active)
{
	bActive = Active;
}

FRotator UComponent_FootIK::NormalToRotator(FVector pVector)
{
	float fAtan2_1 = UKismetMathLibrary::DegAtan2(pVector.Y, pVector.Z);
	float fAtan2_2 = UKismetMathLibrary::DegAtan2(pVector.X, pVector.Z);
	fAtan2_2 *= -1.0f;
	FRotator pResult = FRotator(fAtan2_2, 0.0f, fAtan2_1);

	return pResult;
}