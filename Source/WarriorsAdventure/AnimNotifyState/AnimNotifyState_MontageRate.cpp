
#include "AnimNotifyState_MontageRate.h"

void UAnimNotifyState_MontageRate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	OriginRate = Animation->RateScale;
	Animation->RateScale = Rate;
}

void UAnimNotifyState_MontageRate::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UAnimNotifyState_MontageRate::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Animation->RateScale = OriginRate;
}