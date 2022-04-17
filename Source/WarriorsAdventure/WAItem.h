// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WAItem.generated.h"

UENUM()
enum class EItemCategory
{
	Consumeable,
	Equipment,
	Quest,
	Readables
};

USTRUCT(Blueprintable, BlueprintType)
struct FItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText UseText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeStacked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemCategory Category;
};

UCLASS(Blueprintable)
class WARRIORSADVENTURE_API AWAItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWAItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EventOnUsed();

	virtual FItemInfo GetItemInfo();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", Meta = (AllowPrivateAccess = true))
	FItemInfo ItemInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", Meta = (AllowPrivateAccess = true))
	class AWAInventory* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", Meta = (AllowPrivateAccess = true))
	int32 Index;
};
