// Fill out your copyright notice in the Description page of Project Settings.


#include "WAInventoryWidget.h"
#include "WAInventorySlotWidget.h"
#include "WAInventoryActionMenuWidget.h"
#include "WAWidgetDragDropOperation.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/ScrollBox.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

UWAInventoryWidget::UWAInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UWAInventorySlotWidget> UI_INVENTORY_SLOT(TEXT("/Game/InventorySystem/Widgets/UI_InventorySlot.UI_InventorySlot_C"));
	if (UI_INVENTORY_SLOT.Succeeded())
	{
		InventorySlotWidgetClass = UI_INVENTORY_SLOT.Class;
	}
}

void UWAInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnClicked.AddDynamic(this, &UWAInventoryWidget::CloseInventory);
}

FReply UWAInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

	return EventReply.NativeReply;
}

void UWAInventoryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	FVector2D ScreenSpacePosition = InMouseEvent.GetScreenSpacePosition();
	FVector2D LocalSpacePosition = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, ScreenSpacePosition);

	UWAWidgetDragDropOperation* DragDropOperation =
		Cast<UWAWidgetDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UWAWidgetDragDropOperation::StaticClass()));
	
	DragDropOperation->DefaultDragVisual = this;
	DragDropOperation->Pivot = EDragPivot::MouseDown;
	DragDropOperation->SetWidgetToDrag(this);
	DragDropOperation->SetMouseOffset(LocalSpacePosition);

	RemoveFromParent();

	OutOperation = DragDropOperation;
}

void UWAInventoryWidget::GenerateSlotWidgets()
{
	SlotPanel->ClearChildren();
	InventorySlotWidgets.Empty();

	APlayerController* PlayerController = GetOwningPlayer();
	int32 CurrentSlotIndex = 0;
	for (FInventorySlot InventorySlot : Inventory->GetInventorySlots())
	{
		UWAInventorySlotWidget* InventorySlotWidget = CreateWidget<UWAInventorySlotWidget>(PlayerController, InventorySlotWidgetClass);
		if (InventorySlotWidget)
		{
			InventorySlotWidgets.Add(InventorySlotWidget);
			InventorySlotWidget->SetSlotIndex(CurrentSlotIndex);
			InventorySlotWidget->SetOwningInventory(Inventory);

			int32 GridRow, GridColumn;
			GridRow = CurrentSlotIndex / SlotsPerRow;
			GridColumn = CurrentSlotIndex % SlotsPerRow;
			UUniformGridSlot* UniformGridSlot = SlotPanel->AddChildToUniformGrid(InventorySlotWidget);
			UniformGridSlot->SetRow(GridRow);
			UniformGridSlot->SetColumn(GridColumn);
			InventorySlotWidget->UpdateSlot();
		}

		CurrentSlotIndex += 1;
	}
}

void UWAInventoryWidget::CloseInventory()
{
	Inventory->HideInventory();
}

UWAInventoryActionMenuWidget* UWAInventoryWidget::GetInventoryActionMenu()
{
	return InventoryActionMenu;
}

TArray<class UWAInventorySlotWidget*>& UWAInventoryWidget::GetInventorySlotWidgets()
{
	return InventorySlotWidgets;
}

void UWAInventoryWidget::SetInventory(AWAInventory* _Inventory)
{
	Inventory = _Inventory;
}

void UWAInventoryWidget::SetSlotsPerRow(int32 _SlotsPerRow)
{
	SlotsPerRow = _SlotsPerRow;
}

void UWAInventoryWidget::OnSlotClicked(UWAInventorySlotWidget* InventorySlot)
{
	UUniformGridSlot* UniformGridSlot = UWidgetLayoutLibrary::SlotAsUniformGridSlot(InventorySlot);
	float ActionMenuRow =  FMath::Clamp((UniformGridSlot->Row * 64) - (InventoryScrollBox->GetScrollOffset() / 1.5f), 0.0f, 200.0f);
	float ActionMenuColumn = UniformGridSlot->Column * 64;

	InventoryActionMenu->SetRenderTranslation(FVector2D(ActionMenuColumn, ActionMenuRow));
	InventoryActionMenu->Update(InventorySlot->GetSlotIndex());

	if (InventorySlot->GetInventory()->GetInventorySlots()[InventorySlot->GetSlotIndex()].ItemClass)
	{
		InventoryActionMenu->SetVisibility(ESlateVisibility::Visible);
	}
}
