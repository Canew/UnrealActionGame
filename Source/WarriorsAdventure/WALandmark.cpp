// Fill out your copyright notice in the Description page of Project Settings.


#include "WALandmark.h"
#include "Components/BillboardComponent.h"

// Sets default values
AWALandmark::AWALandmark()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	RootComponent = Billboard;

	Billboard->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));
	if (IconOnRader)
	{
		Billboard->SetSprite(IconOnRader);
	}

}

// Called when the game starts or when spawned
void AWALandmark::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWALandmark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

