// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WAInventoryMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWAInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	class UWAInventoryWidget* GetInventoryWidget();
	void SetInventory(class AWAInventory* _Inventory);
	void SetSlotsPerRow(int32 _SlotsPerRow);
	
private:
	UPROPERTY(Meta = (BindWidget))
	class UWAInventoryWidget* InventoryWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	class AWAInventory* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	int32 SlotsPerRow;
};
