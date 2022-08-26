// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/InventorySlot_W.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Internationalization/TextFormatter.h"
#include "InventorySystem/ItemDrag.h"
#include "InventorySystem/Item/ItemBase.h"
#include "MyProject/LogUtilities.h"

// TODO: Implement the broadcast from UI swapping to Inventory Data swapping and stuffs (might have to double-check circular logic)
// Inventory UI (this) is gonna be 1st in the flow of swapping/move item, then Broadcast() to call other functions to update along 
void UInventorySlot_W::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    Button->OnClicked.AddUniqueDynamic(this, &UInventorySlot_W::UseItem);
    // add `UseItem` to slot's `OnClicked` invocation list
    Button->ClickMethod = EButtonClickMethod::PreciseClick;

    MyProjectCharacter = Cast<AMyProjectCharacter>(GetOwningPlayerPawn());
}

void UInventorySlot_W::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    // set item image to slot only if the Item has the image to offer, and the slot icon is initialized
    if (ItemIcon && Item)
    {
        ItemIcon->SetBrushFromTexture(Item->Thumbnail);
    }

    // Get info (name, desc,... ) from Item (Game-play Time) and set them into widget Tooltip (on item hover)
    if (Item)
    {
        FFormatNamedArguments Args;
        Args.Add("Action", Item->UseActionText);
        Args.Add("Name", Item->DisplayName);
        Args.Add("Description", Item->Description);
        Button->SetToolTipText(FText::Format(FText::FromString("Click to {Action}{Name}\n\n{Description}"), Args));
    }

    // update/synchronize the dimension of the inventory slot for other widget to ref from
    Dimension.X = SizeBox->WidthOverride;
    Dimension.Y = SizeBox->HeightOverride;
}

// Invoke from this UI element's function (like web, user always invoke functionality from front-end)
// From there call the backend similar function for logic & data processing
void UInventorySlot_W::UseItem()
{
    if (MyProjectCharacter != nullptr)
    {
        MyProjectCharacter->UseItem(Item);
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("UInventorySlot_W::UseItem(): MyProjectCharacter is null!"))
}

void UInventorySlot_W::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                         UDragDropOperation* InOperation)
{
    Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
    // Button->WidgetStyle = DragOverStyle;
}

void UInventorySlot_W::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragLeave(InDragDropEvent, InOperation);
    // Button->WidgetStyle = DefaultButtonStyle;
}

FReply UInventorySlot_W::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
    const FEventReply reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

    return reply.NativeReply;
}

void UInventorySlot_W::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                            OUT UDragDropOperation*& OutOperation)
{
    VSCREENMSG(FColor::Orange, "Enter Drag Detected");
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
    // if it's an empty slot -> we cannot drag it
    if (!Item)
    {
        return;
    }

    // create new widget
    auto DragWidget = CreateWidget<UDraggedItem_W>(GetOwningPlayer(), DraggedItemWidgetClass);
    DragWidget->DraggedImage = Item->Thumbnail;

    // create item Drag & Drop Operation
    UItemDrag* ItemDrag = NewObject<UItemDrag>();
    ItemDrag->Pivot = EDragPivot::MouseDown;
    ItemDrag->ItemImage = Item->Thumbnail;
    ItemDrag->DefaultDragVisual = DragWidget;
    ItemDrag->SlotIndex = IndexInInventory_1D;

    //get data from item to ItemDrag->DraggedItem for ::NativeOnDrop
    ItemDrag->DraggedItem = Item;

    OUT OutOperation = ItemDrag;
}

// TODO: Finish `UInventorySlot_WC::NativeOnDrop` function
bool UInventorySlot_W::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                    UDragDropOperation* InOperation)
{
    if (auto ItemDrag = Cast<UItemDrag>(InOperation))
    {
        if (!ItemDrag->bIsFromInventoryMenu)
        {
            goto end_function;
        }
        OnInventorySwap.Broadcast(ItemDrag->SlotIndex, IndexInInventory_1D);
    }

end_function:
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
