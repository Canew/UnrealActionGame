// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_GetRandomValue.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UBTS_GetRandomValue : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_GetRandomValue();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 Min;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 Max;
};
