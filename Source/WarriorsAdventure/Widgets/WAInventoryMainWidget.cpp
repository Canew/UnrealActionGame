// Fill out your copyright notice in the Description page of Project Settings.


#include "WAInventoryMainWidget.h"
#include "WAInventory.h"
#include "WAInventoryWidget.h"
#include "WAInventoryActionMenuWidget.h"
#include "WAWidgetDragDropOperation.h"
#include "Blueprint/SlateBlueprintLibrary.h"

void UWAInventoryMainWidget::NativeConstruct()
{
	InventoryWidget->SetInventory(Inventory);
	InventoryWidget->SetSlotsPerRow(SlotsPerRow);
	InventoryWidget->GenerateSlotWidgets();
	InventoryWidget->GetInventoryActionMenu()->SetInventory(Inventory);
}

bool UWAInventoryMainWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UWAWidgetDragDropOperation* WidgetDragDropOperation = Cast<UWAWidgetDragDropOperation>(InOperation);

	if (WidgetDragDropOperation)
	{
		FVector2D InventoryPosition = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, InDragDropEvent.GetScreenSpacePosition())
			- WidgetDragDropOperation->GetMouseOffset();

		WidgetDragDropOperation->GetWidgetToDrag()->AddToViewport();
		WidgetDragDropOperation->GetWidgetToDrag()->SetPositionInViewport(InventoryPosition, false);

		return true;
	}
	else
	{
		return false;
	}
}

class UWAInventoryWidget* UWAInventoryMainWidget::GetInventoryWidget()
{
	return InventoryWidget;
}

void UWAInventoryMainWidget::SetInventory(AWAInventory* _Inventory)
{
	Inventory = _Inventory;
}

void UWAInventoryMainWidget::SetSlotsPerRow(int32 _SlotsPerRow)
{
	SlotsPerRow = _SlotsPerRow;
}
