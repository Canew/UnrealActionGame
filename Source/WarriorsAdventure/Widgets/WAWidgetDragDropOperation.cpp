// Fill out your copyright notice in the Description page of Project Settings.


#include "WAWidgetDragDropOperation.h"
#include "Blueprint/UserWidget.h"

void UWAWidgetDragDropOperation::SetWidgetToDrag(UUserWidget* _WidgetToDrag)
{
	WidgetToDrag = _WidgetToDrag;
}

void UWAWidgetDragDropOperation::SetMouseOffset(FVector2D _MouseOffset)
{
	MouseOffset = _MouseOffset;
}

UUserWidget* UWAWidgetDragDropOperation::GetWidgetToDrag()
{
	return WidgetToDrag;
}

FVector2D UWAWidgetDragDropOperation::GetMouseOffset()
{
	return MouseOffset;
}
