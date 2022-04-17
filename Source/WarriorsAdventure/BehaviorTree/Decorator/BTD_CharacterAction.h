// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorsAdventure.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BTD_CharacterAction.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UBTD_CharacterAction : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_CharacterAction();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
private:
	UPROPERTY(Category = "Character Action", EditAnywhere, meta = (DisplayName = "Key Query"))
	TEnumAsByte<EBasicKeyOperation::Type> BasicOperation;

	UPROPERTY(Category = "Character Action", EditAnywhere, meta = (DisplayName = "Character Action"))
	ECharacterAction CharacterAction;
};
