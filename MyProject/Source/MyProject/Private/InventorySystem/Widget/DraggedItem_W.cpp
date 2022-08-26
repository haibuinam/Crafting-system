// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/DraggedItem_W.h"

void UDraggedItem_W::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    // DraggedItem = GetImage_DraggedItem();
    // TODO: get the image from the item we are aiming to drag
    // DraggedItem->SetBrushFromTexture();
    // Image->SetBrushFromTexture(DraggedImage);
}

void UDraggedItem_W::NativeConstruct()
{
    Super::NativeConstruct();
}
