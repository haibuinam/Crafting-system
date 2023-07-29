// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/Inventory_W.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridSlot.h"
#include "Components/WrapBox.h"
#include "InventorySystem/WidgetDrag.h"
#include "InventorySystem/Inventory/InventoryComponent.h"
#include "InventorySystem/Item/ItemBase.h"
#include "MyProject/LogUtilities.h"
#include "MyProject/MyProjectCharacter.h"

#define OUT 

// TODO: figure out a way to Synchronize between number of slots in design and Capacity
void UInventory_W::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    /*-----GET AND STORE DATA-----*/

    // TODO: later change the storage of HUD into player controller instead of player
    // Currently leave it here because I haven't override `GameMode` with MyPlayerController
    MyPlayerController = Cast<AMyPlayerController>(GetOwningPlayer());

    MyProjectCharacter = Cast<AMyProjectCharacter>(GetOwningPlayerPawn());

    HUDReference = MyProjectCharacter->HUD_Widget;

    // TODO: change Inventory Owner from `Character` to `PlayerState` when implementing Server-Client model
    Inventory = MyProjectCharacter->Inventory;
    Inventory->OnInventoryUpdated.AddUniqueDynamic(this, &UInventory_W::UpdateSlotUI);
    // add to OnInventoryUpdate invocation list

    /*-----INIT THE UI-----*/

    // UI Init 
    InitInventoryUI();
    UpdateInventoryUI();

    // Display Settings
    Border->Visibility = ESlateVisibility::Hidden; // hidden by default
    bIsFocusable = true; // set widget focusable

    // function binding
    CloseButton->OnClicked.AddUniqueDynamic(MyProjectCharacter, &AMyProjectCharacter::CloseInventory);
    // add `Close Inventory` to Slot invocation list
}

void UInventory_W::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    // we only want to change and preview the design at Design Time only (in Editor)
    if (IsDesignTime())
    {
        InitInventoryUI();

        CalculateWidgetDimension();

        // set the proper width for SizeBox
        SizeBox->SetHeightOverride(Dimension.Y);
        SizeBox->SetWidthOverride(Dimension.X);

        // update visibility of this widget
        this->SetVisibility(ESlateVisibility::Visible);

        // set the display to Desire On Screen
        //this->DesignSizeMode = EDesignPreviewSizeMode::DesiredOnScreen;

        // this only for later reference from other widgets or for your own sake
        NumOfSlot = InventoryBoxMaxRow * InventoryBoxMaxCol;
    }
    // else in game-play, since the size is fixed, we don't have to do any recalculation and resizing
    // only needs to update/sync the data with in-game actions, which is already bind-ed here: link:Inventory_W.cpp:"Inventory->OnInventoryUpdated.AddUniqueDynamic(this, &UInventory_W::UpdateSlotUI);"
}

FReply UInventory_W::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    const auto CurrentWorldPosition = InMouseEvent.GetScreenSpacePosition();
    Offset = InGeometry.AbsoluteToLocal(CurrentWorldPosition);

    const auto EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
    return EventReply.NativeReply;
}

void UInventory_W::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                        OUT UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

    const auto NewWidgetDragOperation = NewObject<UWidgetDrag>();
    NewWidgetDragOperation->WidgetToDrag = this;
    NewWidgetDragOperation->DefaultDragVisual = this;
    NewWidgetDragOperation->DropWindowOffset = Offset;
    NewWidgetDragOperation->Pivot = EDragPivot::MouseDown;

    this->RemoveFromParent();

    OUT OutOperation = NewWidgetDragOperation;
}

// create empty slot widgets in the Inventory Widget
void UInventory_W::InitInventoryUI()
{
    if (!InventorySlotClass)
    {
        return;
    }

    InventoryBox->ClearChildren();

    for (int row = 0; row < InventoryBoxMaxRow; row++)
    {
        for (int col = 0; col < InventoryBoxMaxCol; col++)
        {
            // init the inventory slot widget
            const auto InventorySlot_Widget = CreateWidget<UInventorySlot_W>(this, InventorySlotClass);

            // TODO P4: Think of optimizing this. This one is begin re-assigned every iteration (which is redundant)
            InventorySlotSize = InventorySlot_Widget->Dimension;

            // init the slot as a default blank slot
            InventorySlot_Widget->IndexInInventory_1D = row * InventoryBoxMaxCol + col;
            InventorySlot_Widget->ItemAmount->SetVisibility(ESlateVisibility::Hidden);

            // Bind swapping-related functions to each slot
            if (!IsDesignTime())
            {
                // only when we are playing to we want ot bind swapping functions
                InventorySlot_Widget->OnInventorySwap.AddUniqueDynamic(this, &UInventory_W::SwapSlotWidgets);
                // add the UI swap
                InventorySlot_Widget->OnInventorySwap.AddUniqueDynamic(
                    Inventory, &UInventoryComponent::SwapInventoryItems); // add the data swap
            }

            // add to UI
            UUniformGridSlot* GridSlot = InventoryBox->AddChildToUniformGrid(InventorySlot_Widget, row, col);

            InventorySlot_Widgets.Add(GridSlot); // add to the list of inventory slot panels 

            // set the grid slot styling
            GridSlot->SetVerticalAlignment(VAlign_Center);
            GridSlot->SetHorizontalAlignment(HAlign_Center);
        }
    }
}

// sync with the data array (Items[] from InventoryComponent), since the Init..() function just for creating empty slots in the Inventory WidgetL
// link:Inventory_W.cpp:"InitInventoryUI()" 
void UInventory_W::UpdateInventoryUI()
{
    for (int i = 0; i < Inventory->Items.Num(); i++)
    {
        UpdateSlotUI(Inventory->Items[i], i);
    }
}

// TODO: Make stack-able items stacking when pickup
void UInventory_W::UpdateSlotUI(AItemBase* ItemToUpdate, int32 ItemIndex)
{
    UInventorySlot_W* InventorySlot_Widget = Cast<UInventorySlot_W>(InventorySlot_Widgets[ItemIndex]->Content);
    // store in a var for optimization

    if (InventorySlot_Widget == nullptr)
    {
        V_LOG("Casting failed");
        return;
    }

    // if item at ItemIndex of the Inventory is NULL, we set the slot back to default
    if (ItemToUpdate == nullptr)
    {
        V_LOG("ItemToUpdate is NULL");
        // just reset everything to default (as this item could possible be null from a `remove` operation)
        InventorySlot_Widget->ItemIcon->SetBrushFromTexture(nullptr);
        InventorySlot_Widget->ItemAmount->Visibility = ESlateVisibility::Hidden;
    }
    else // we will set the item the corresponding ItemIcon
    {
        V_LOG("ItemToUpdate is FOUND");
        // TODO: update all field from Item into Slot Widget
        InventorySlot_Widget->Item = ItemToUpdate;
        InventorySlot_Widget->ItemIcon->SetBrushFromTexture(ItemToUpdate->Thumbnail);
        //InventorySlot_Widget->ItemAmount->Visibility = ESlateVisibility::Visible;
        InventorySlot_Widget->ItemAmount->SetText(FText::FromString("x" + FString::FromInt(ItemToUpdate->Amount)));
        // display amount if the item count in the Inventory > 1
        if (ItemToUpdate->Amount > 1)
        {
            InventorySlot_Widget->ItemAmount->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            InventorySlot_Widget->ItemAmount->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UInventory_W::SwapSlotWidgets(const int32& IndexFrom, const int32& IndexTo)
{
    // Swap on Uniform Grid Slots
    auto UniformGridSlot_From = InventorySlot_Widgets[IndexFrom];
    UniformGridSlot_From->SetRow(IndexTo / InventoryBoxMaxCol);
    UniformGridSlot_From->SetColumn(IndexTo % InventoryBoxMaxCol);

    auto UniformGridSlot_To = InventorySlot_Widgets[IndexTo];
    UniformGridSlot_To->SetRow(IndexFrom / InventoryBoxMaxCol);
    UniformGridSlot_To->SetColumn(IndexFrom % InventoryBoxMaxCol);

    // Swap Index Of 
    if (auto InventorySlot_W_From = Cast<UInventorySlot_W>(UniformGridSlot_From->Content))
    {
        InventorySlot_W_From->IndexInInventory_1D = IndexTo;
    }

    if (auto InventorySlot_W_To = Cast<UInventorySlot_W>(UniformGridSlot_To->Content))
    {
        InventorySlot_W_To->IndexInInventory_1D = IndexFrom;
    }

    // Swap Pointer Locations Only (data attached is kept same)
    InventorySlot_Widgets.Swap(IndexFrom, IndexTo);
}

void UInventory_W::CalculateWidgetDimension()
{
    Dimension.X = InventoryBoxMaxCol * (InventorySlotSize.X + InventoryBox->SlotPadding.Left + InventoryBox->SlotPadding
        .Right) + SizeBoxOverrideExtraSpace.Horizontal;
    Dimension.Y = InventoryBoxMaxRow * (InventorySlotSize.Y + InventoryBox->SlotPadding.Top + InventoryBox->SlotPadding.
        Bottom) + SizeBoxOverrideExtraSpace.Vertical;
}

