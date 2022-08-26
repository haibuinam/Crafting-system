// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/ItemCraftingSlot_WC.h"

//#include "InventorySystem/ItemDrag.h"
#include "ImageUtils.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Internationalization/TextFormatter.h"
#include "InventorySystem/ItemDrag.h"
#include "InventorySystem/Item/ItemBase.h"
#include "MyProject/LogUtilities.h"
//#include "ItemCraftingSlot_WC.h"


// TODO: Implement the broadcast from UI swapping to Inventory Data swapping and stuffs (might have to double-check circular logic)
// Inventory UI (this) is gonna be 1st in the flow of swapping/move item, then Broadcast() to call other functions to update along 
void UItemCraftingSlot_WC::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Button->OnClicked.AddUniqueDynamic(this, &UItemCraftingSlot_WC::SendNameItemWhenPlayerClicked);

    //MyProjectCharacter = Cast<AMyProjectCharacter>(GetOwningPlayerPawn());
}

void UItemCraftingSlot_WC::SynchronizeProperties()
{
    Super::SynchronizeProperties();
    // set item image to slot only if the Item has the image to offer, and the slot icon is initialized
    GetDefaultIcon();
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

    if (!DraggedItemWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("DraggedItemWidgetClass is FALSE "));
        return;
    }
    // update/synchronize the dimension of the inventory slot for other widget to ref from
    Dimension.X = SizeBox->WidthOverride;
    Dimension.Y = SizeBox->HeightOverride;
}

void UItemCraftingSlot_WC::GetDefaultIcon()
{
    const FString PathImage = "ImageCrafting/";
    FString FullPath = FPaths::ProjectContentDir() + PathImage + "black circle.png";
    ItemIcon->SetBrushFromTexture(FImageUtils::ImportFileAsTexture2D(FullPath));
    if(!ItemIcon)
    {
        V_LOG("Item Icon 2D is nullptr");
    }
}

void UItemCraftingSlot_WC::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
    // Button->WidgetStyle = DragOverStyle;
}

void UItemCraftingSlot_WC::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragLeave(InDragDropEvent, InOperation);
    // Button->WidgetStyle = DefaultButtonStyle;
}

FReply UItemCraftingSlot_WC::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
    const FEventReply reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

    return reply.NativeReply;
}

void UItemCraftingSlot_WC::SendNameItemWhenPlayerClicked()
{
    OnClickedItemCraftingButton.Broadcast(Item, LocationVector2DWidget);
}

void UItemCraftingSlot_WC::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
    OUT UDragDropOperation*& OutOperation)
{
    VSCREENMSG(FColor::Orange, "CRAFTING: Enter Drag Detected");
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
    ItemDrag->SlotIndex = IndexInCrafting;

    //get data from item to ItemDrag->DraggedItem for ::NativeOnDrop
    ItemDrag->DraggedItem = Item;

    OUT OutOperation = ItemDrag;
}

// TODO: Finish `UItemCrafting_WC::NativeOnDrop` function
bool UItemCraftingSlot_WC::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    if (auto ItemDrag = Cast<UItemDrag>(InOperation))
    {
        //UE_LOG(LogTemp, Warning, TEXT("ItemDrag->SlotIndex: %d"),ItemDrag->SlotIndex);
        //UE_LOG(LogTemp, Warning, TEXT("IndexInInventory_1D: %d"), IndexInCrafting);

        //UE_LOG(LogTemp, Warning, TEXT("Item amount: %d"),ItemDrag->DraggedItem->Amount);

        OnCraftingAddWidget.Broadcast(ItemDrag, IndexInCrafting);
    }
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

