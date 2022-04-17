// Fill out your copyright notice in the Description page of Project Settings.


#include "WAPickupItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "WAPlayerCharacter.h"
#include "WAInventory.h"

// Sets default values
AWAPickupItem::AWAPickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SPHERE(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SM_SPHERE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_SPHERE.Object);
	}
	RootComponent = Mesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetSphereRadius(100.0f);
	Sphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AWAPickupItem::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AWAPickupItem::OnSphereBeginOverlap);
}

// Called every frame
void AWAPickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWAPickupItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWAInventory* Inventory = Cast<AWAPlayerCharacter>(OtherActor)->GetInventory();
	if (Inventory && ItemToAdd)
	{
		int32 Rest;
		if (Inventory->AddItem(Cast<AWAItem>(ItemToAdd->GetDefaultObject()), AmountToAdd, Rest))
		{
			if (Rest > 0)
			{
				AmountToAdd = Rest;
			}
			else
			{
				Destroy();
			}
		}
	}
}