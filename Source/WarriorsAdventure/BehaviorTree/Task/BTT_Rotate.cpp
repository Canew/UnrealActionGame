// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Rotate.h"
#include "WACharacter.h"
#include "WAAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_Rotate::UBTT_Rotate()
{
	NodeName = TEXT("Rotate");
}

EBTNodeResult::Type UBTT_Rotate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto WAAIController = Cast<AWAAIController>(OwnerComp.GetAIOwner());
	if (WAAIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	auto WACharacter = Cast<AWACharacter>(WAAIController->GetPawn());
	if (WACharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<AWACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
	if (Target == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector LookVector = Target->GetActorLocation() - WACharacter->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRotator = FRotationMatrix::MakeFromX(LookVector).Rotator();
	WACharacter->SetActorRotation(FMath::RInterpTo(WACharacter->GetActorRotation(), TargetRotator, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}