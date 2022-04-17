// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WAInteractComponent.h"
#include "WACharacter.h"
#include "WAPlayerController.h"

// Sets default values for this component's properties
UWAInteractComponent::UWAInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;
}


// Called when the game starts
void UWAInteractComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UWAInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindInteractableActor();
}

TWeakObjectPtr<AActor> UWAInteractComponent::GetInteractableActor()
{
	return InteractableActor;
}

void UWAInteractComponent::FindInteractableActor()
{
	AWACharacter* pCharacter = Cast<AWACharacter>(GetOwner());

	TArray<FOverlapResult> OverlapResults;
	GetWorld()->OverlapMultiByChannel(OverlapResults, pCharacter->GetActorLocation(), pCharacter->GetActorRotation().Quaternion(),
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(1.5f * pCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	if (OverlapResults.Num() > 0)
	{
		TWeakObjectPtr<AActor> pActor = (*(OverlapResults.begin())).Actor;
		if (pActor.IsValid())
		{
			InteractableActor = pActor;
			AWAPlayerController* pPlayerController = Cast<AWAPlayerController>(pCharacter->GetController());
			if (pPlayerController)
			{
				pPlayerController->DisplayInteractWidget(true);
			}
		}
	}
	else
	{
		InteractableActor = nullptr;
		AWAPlayerController* pPlayerController = Cast<AWAPlayerController>(pCharacter->GetController());
		if (pPlayerController)
		{
			pPlayerController->DisplayInteractWidget(false);
		}
	}
}

