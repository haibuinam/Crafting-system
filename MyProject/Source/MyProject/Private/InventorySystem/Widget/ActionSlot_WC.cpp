// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/ActionSlot_WC.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "InventorySystem/ItemDrag.h"

#define OUT

void UActionSlot_WC::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Visibility = ESlateVisibility::Visible;

    DesignSizeMode = EDesignPreviewSizeMode::DesiredOnScreen;

    Button->SetClickMethod(EButtonClickMethod::PreciseClick);
    Button->IsFocusable = false;
}

void UActionSlot_WC::NativeConstruct()
{
    Super::NativeConstruct();
}

void UActionSlot_WC::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    // set the default style based on what type of slot it is
    SetDefaultButton();

    // update dimension
    Dimension.X = SizeBox->WidthOverride;
    Dimension.Y = SizeBox->HeightOverride;
}

FSlateBrush UActionSlot_WC::GetBrush()
{
    return UWidgetBlueprintLibrary::MakeBrushFromTexture(ActionImage);
}

void UActionSlot_WC::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                       UDragDropOperation* InOperation)
{
    Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
    Button->WidgetStyle = DragOverStyle;
}

void UActionSlot_WC::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragLeave(InDragDropEvent, InOperation);
    Button->WidgetStyle = DefaultButtonStyle;
}

FReply UActionSlot_WC::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    const FEventReply reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

    return reply.NativeReply;
}

void UActionSlot_WC::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                          OUT UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
    // if it's an empty slot -> we cannot drag it
    if (!ActionImage)
    {
        return;
    }

    // create new widget
    auto DragWidget = CreateWidget<UDraggedItem_W>(GetOwningPlayer(), DraggedItemWidgetClass);
    DragWidget->DraggedImage = ActionImage;

    // create item Drag & Drop Operation
    UItemDrag* ItemDrag = NewObject<UItemDrag>();
    ItemDrag->Pivot = EDragPivot::MouseDown;
    ItemDrag->ItemImage = ActionImage;
    ItemDrag->DefaultDragVisual = DragWidget;
    ItemDrag->SlotIndex = ActionSlotIndex;

    OUT OutOperation = ItemDrag;
}

// TODO: Finish `UActionSlot_WC::NativeOnDrop` function
bool UActionSlot_WC::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                  UDragDropOperation* InOperation)
{
    if (auto ItemDrag = Cast<UItemDrag>(InOperation))
    {
    }

    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UActionSlot_WC::SetDefaultButton()
{
    if (Button)
    {
        if (ActionImg)
        {
            Button->WidgetStyle = EnabledButtonStyle;
        }
        else
        {
            Button->WidgetStyle = DefaultButtonStyle;
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UActionSlot_WC::SetDefaultButton(): Slot is nullptr"));
    }
}
