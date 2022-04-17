// Fill out your copyright notice in the Description page of Project Settings.


#include "WASecret.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WARadarComponent.h"

// Sets default values
AWASecret::AWASecret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	HighlightRange = CreateDefaultSubobject<USphereComponent>(TEXT("HighlightRange"));
	HighlightRange->SetupAttachment(RootComponent);

	HighlightRange->SetSphereRadius(HighlightRadius, true); 
}

// Called when the game starts or when spawned
void AWASecret::BeginPlay()
{
	Super::BeginPlay();
	
	HighlightRange->OnComponentBeginOverlap.AddDynamic(this, &AWASecret::OnHighlightRangeBeginOverlap);
	HighlightRange->OnComponentEndOverlap.AddDynamic(this, &AWASecret::OnHighlightRangeEndOverlap);
}

void AWASecret::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	HighlightRange->SetSphereRadius(HighlightRadius, true);
}

// Called every frame
void AWASecret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWASecret::OnHighlightRangeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UWARadarComponent* RadarComponent = Cast<UWARadarComponent>(OtherActor->GetComponentByClass(UWARadarComponent::StaticClass()));
	if (RadarComponent)
	{
		RadarComponent->OnEnterSecretRange(this);
	}
}

void AWASecret::OnHighlightRangeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	UWARadarComponent* RadarComponent = Cast<UWARadarComponent>(OtherActor->GetComponentByClass(UWARadarComponent::StaticClass()));
	if (RadarComponent)
	{
		RadarComponent->OnLeaveSecretRange(this);
	}
}