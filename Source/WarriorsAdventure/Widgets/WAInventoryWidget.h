// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WAInventory.h"
#include "WAInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSADVENTURE_API UWAInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UWAInventoryWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	void GenerateSlotWidgets();
	UFUNCTION()
	void CloseInventory();

	class UWAInventoryActionMenuWidget* GetInventoryActionMenu();
	TArray<class UWAInventorySlotWidget*>& GetInventorySlotWidgets();
	void SetInventory(class AWAInventory* _Inventory);
	void SetSlotsPerRow(int32 _SlotsPerRow);

	void OnSlotClicked(class UWAInventorySlotWidget* InventorySlot);
	
private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* CloseButton;

	UPROPERTY(Meta = (BindWidget))
	class UUniformGridPanel* SlotPanel;

	UPROPERTY(Meta = (BindWidget))
	class UScrollBox* InventoryScrollBox;

	UPROPERTY(Meta = (BindWidget))
	class UWAInventoryActionMenuWidget* InventoryActionMenu;

	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess))
	class AWAInventory* Inventory;

	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess))
	int32 SlotsPerRow = 4;

	TSubclassOf<class UWAInventorySlotWidget> InventorySlotWidgetClass;
	TArray<class UWAInventorySlotWidget*> InventorySlotWidgets;
};
