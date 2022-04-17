// Fill out your copyright notice in the Description page of Project Settings.


#include "WABonfire.h"

// Sets default values
AWABonfire::AWABonfire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Bonfire = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bonfire"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WOODENLAMP(TEXT("StaticMesh'/Game/DungeonLamps/Meshes/SM_FireBasekd_B_002.SM_FireBasekd_B_002'"));
	if (SM_WOODENLAMP.Succeeded())
	{
		Bonfire->SetStaticMesh(SM_WOODENLAMP.Object);
	}
	Bonfire->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));
	SetRootComponent(Bonfire);

	FireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticle"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_FIRE(TEXT("ParticleSystem'/Game/DungeonLamps/Particles/P_Fire_for_Tortches.P_Fire_for_Tortches'"));
	if (P_FIRE.Succeeded())
	{
		FireParticle->SetTemplate(P_FIRE.Object);
	}
	FireParticle->SetRelativeLocation(FVector(0.0f, 0.0f, 1.1f * Bonfire->GetStaticMesh()->GetBounds().GetBox().GetSize().Z));
	FireParticle->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
	FireParticle->bAutoActivate = false;
	FireParticle->SetupAttachment(RootComponent);

	Bonfire->SetCollisionProfileName(TEXT("WAInteractableActor"));
}

// Called when the game starts or when spawned
void AWABonfire::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AWABonfire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWABonfire::BeInteracted(AActor* InteractCauser)
{
	Activate();
}

void AWABonfire::Activate()
{
	bActivated = true;
	FireParticle->Activate(true);
}

