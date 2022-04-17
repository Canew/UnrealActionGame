// Fill out your copyright notice in the Description page of Project Settings.


#include "WAPlayerCharacter.h"
#include "WAPlayerController.h"
#include "Components/WAInteractComponent.h"
#include "WAInteractInterface.h"
#include "Components/WARadarComponent.h"
#include "MapViewComponent.h"
#include "WAInventory.h"

AWAPlayerCharacter::AWAPlayerCharacter()
{
	// Create SpringArm and Camera.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->TargetArmLength = 600.0f;
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	SpringArm->bEnableCameraLag = true;

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	// Create Interact Component.
	InteractComponent = CreateDefaultSubobject<UWAInteractComponent>(TEXT("InteractComponent"));

	// Create Radar
	CharacterRadar = CreateDefaultSubobject<UWARadarComponent>(TEXT("Radar"));

	// Create MapViewComponent
	MapViewComponent = CreateDefaultSubobject<UMapViewComponent>(TEXT("MapView"));
	MapViewComponent->SetupAttachment(RootComponent);
	MapViewComponent->SetZoomScale(10.0f);
	
}

void AWAPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterRadar->InitializeRadar();

	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CharacterInventory = Cast<AWAInventory>(GetWorld()->SpawnActor<AWAInventory>(ActorSpawnParameters));
	CharacterInventory->SetCharacterRef(this);
	CharacterInventory->SetAmountOfSlots(20);
	CharacterInventory->SetSlotsPerRow(4);
	CharacterInventory->CreateInventory();
}

void AWAPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If target locked, look at the target.
	if (bTargetLock)
	{
		if (LockedTarget.IsValid())
		{
			// If either the target is far away or not visible from camera, target lock is unlocked.
			if ((GetActorLocation() - LockedTarget->GetActorLocation()).Size() >= TargetLockRange || !GetController()->LineOfSightTo(LockedTarget.Get(), Camera->GetComponentLocation()))
			{
				TargetUnlock();
			}
			else
			{
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Camera->GetComponentLocation(), LockedTarget->GetActorLocation());
				LookAtRotation = FMath::RInterpTo(GetControlRotation(), LookAtRotation, DeltaTime, 10.0f);
				FRotator NewRotation = FRotator(GetControlRotation().Pitch, LookAtRotation.Yaw, GetControlRotation().Roll);
				GetController()->SetControlRotation(NewRotation);
			}
		}
		else
		{
			TargetUnlock();
		}
	}
}

void AWAPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AWAPlayerCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AWAPlayerCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AWAPlayerCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AWAPlayerCharacter::LookUp);

	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &AWAPlayerCharacter::Dash);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AWAPlayerCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AWAPlayerCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Released, this, &AWAPlayerCharacter::AttackReleased);
	PlayerInputComponent->BindAction(TEXT("TargetLock"), EInputEvent::IE_Pressed, this, &AWAPlayerCharacter::TargetLock);
	PlayerInputComponent->BindAction(TEXT("R"), EInputEvent::IE_Pressed, this, &AWAPlayerCharacter::Ultimate);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AWAPlayerCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("ToggleUI"), EInputEvent::IE_Pressed, this, &AWAPlayerCharacter::ToggleUI);
	PlayerInputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &AWAPlayerCharacter::ToggleInventory);
}

void AWAPlayerCharacter::UpDown(float NewAxisValue)
{
	Super::UpDown(NewAxisValue);

	if (NewAxisValue != 0.0f)
	{
		CharacterRadar->OnPlayerMoved();
	}
}

void AWAPlayerCharacter::LeftRight(float NewAxisValue)
{
	Super::LeftRight(NewAxisValue);

	if (NewAxisValue != 0.0f)
	{
		CharacterRadar->OnPlayerMoved();
	}
}

void AWAPlayerCharacter::Turn(float NewAxisValue)
{
	Super::Turn(NewAxisValue);

	if (NewAxisValue != 0.0f)
	{
		CharacterRadar->OnPlayerTurned();
	}
}

void AWAPlayerCharacter::Interact()
{
	TWeakObjectPtr<AActor> InteractableActor = InteractComponent->GetInteractableActor();
	if (InteractComponent->GetInteractableActor().IsValid())
	{
		IWAInteractInterface* InteractInterface = Cast<IWAInteractInterface>(InteractableActor.Get());
		if (InteractInterface)
		{
			InteractInterface->BeInteracted(this);
		}
	}
}

void AWAPlayerCharacter::TargetLock()
{
	if (bTargetLock)
	{
		if (TargetSphere.IsValid())
		{
			TargetUnlock();
		}
	}
	else
	{
		FHitResult HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);
		bool bResult = GetWorld()->SweepSingleByChannel(
			HitResult,
			GetActorLocation(),
			GetActorLocation() + (TargetLockRange * FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X)),
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2,
			FCollisionShape::MakeSphere(250.0f),
			Params);

		if (bResult)
		{
			if (HitResult.Actor.IsValid())
			{
				bTargetLock = true;

				// Set target sphere class
				UObject* ObjectToSpawn = StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Blueprints/BP_TargetSphere.BP_TargetSphere'"));
				UBlueprint* GeneratedBP = Cast<UBlueprint>(ObjectToSpawn);
				UClass* ClassToSpawn = ObjectToSpawn->StaticClass();

				// Spawn target sphere.
				const FVector SpawnLocation = HitResult.Actor->GetActorLocation();
				const FRotator SpawnRotation = FRotator::ZeroRotator;
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				TargetSphere = GetWorld()->SpawnActor(GeneratedBP->GeneratedClass, &SpawnLocation, &SpawnRotation, SpawnParams);

				LockedTarget = Cast<AWACharacter>(HitResult.Actor);

				TargetSphere->AttachToActor(LockedTarget.Get(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			}
		}
	}
}

void AWAPlayerCharacter::TargetUnlock()
{
	bTargetLock = false;
	TargetSphere->Destroy();
	TargetSphere.Reset();
	LockedTarget.Reset();
}

void AWAPlayerCharacter::ToggleUI()
{
	CharacterRadar->ToggleVisibility(true);
}

void AWAPlayerCharacter::ToggleInventory()
{
	if (CharacterInventory->IsVisible())
	{
		CharacterInventory->HideInventory();
	}
	else
	{
		CharacterInventory->ShowInventory();
	}
}

AWAInventory* AWAPlayerCharacter::GetInventory()
{
	return CharacterInventory;
}
