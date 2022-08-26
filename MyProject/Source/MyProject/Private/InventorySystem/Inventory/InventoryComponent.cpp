// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Inventory/InventoryComponent.h"

#include "InventorySystem/Item/ItemBase.h"
#include "MyProject/LogUtilities.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
    DefaultItems.Init(nullptr, Capacity);
    Items.Init(nullptr, Capacity);
}


// Called when the game starts
// TODO: this must be done on Server
void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    // init current with default items
    for (int i = 0; i < DefaultItems.Num(); i++)
    {
        Items[i] = DefaultItems[i];
    }
}

// TODO: extract this to be done on Server
bool UInventoryComponent::AddItem(AItemBase* Item)
{
    if (!Item)
    {
        V_LOG("Item is null");
        return false;
    }

    if (IsFull())
    {
        V_LOG("Inventory is full");
        return false;
    }

    int32 ItemIndex = INDEX_NONE;
    // linear-search the item
    for (int i = 0; i < Items.Num(); i++)
    {
        if (Items[i] != nullptr)
        {
            if (Items[i]->operator==(Item))
            {
                ItemIndex = i;
                break;
            }
        }
    }

    // update container data
    // if the ItemIndex == -1, it doesn't matter if the item is stack-able or not, we will put item into new slot
    if (ItemIndex == INDEX_NONE || !Item->bIsStackable)
    {
        ItemIndex = GetFirstEmptyItemIndex();
        // this Item has not been in container, assign new position + amount to 1
        Items[ItemIndex] = Item;
        Items[ItemIndex]->Amount = 1;
        Size++;
    }
    else
    {
        Items[ItemIndex]->Amount += 1;
    }
    // khanhcd modified end 

    // broadcast to update UI link:Inventory_W.cpp:"void UInventory_W::UpdateSlotUI(AItemBase* ItemToUpdate, int32 ItemIndex)"
    OnInventoryUpdated.Broadcast(Items[ItemIndex], ItemIndex);

    // assign item metadata
    Item->OwningInventory = this;
    Item->World = GetWorld();
    Item->Index1D = ItemIndex;

    return true;
}

// TODO: this must be done on Server
bool UInventoryComponent::RemoveItem(AItemBase* Item)
{
    if (!Item)
    {
        V_LOG("Item does NOT EXIST");
        return false;
    }

    if (IsEmpty())
    {
        V_LOG("Inventory is Empty!");
        return false;
    }

    int32 ItemIndex = Items.Find(Item);

    if (ItemIndex == INDEX_NONE)
    {
        V_LOG("No such item to remove");
        return false;
    }
    
    // update container data
    if (Items[ItemIndex]->Amount == 1)
    {
        // remove item metadata
        Item->OwningInventory = nullptr;
        Item->World = nullptr;
        Items.RemoveAt(ItemIndex, 1, false);
        if (Items[ItemIndex] != nullptr)
        {
            Items[ItemIndex] = nullptr;
        }
        Size--;
    }
    else
    {
        Items[ItemIndex]->Amount -= 1;
    }

    // broadcast to update UI link:Inventory_W.cpp:"void UInventory_W::UpdateSlotUI(AItemBase* ItemToUpdate, int32 ItemIndex)"
    OnInventoryUpdated.Broadcast(Items[ItemIndex], ItemIndex);

    return true;
}

bool UInventoryComponent::IsFull() const
{
    return Size == Items.Num();
}

bool UInventoryComponent::IsEmpty() const
{
    return Size == 0;
}

int32 UInventoryComponent::GetFirstEmptyItemIndex()
{
    for (int i = 0; i < Capacity; i++)
    {
        if (Items[i] == nullptr)
        {
            return i;
        }
    }

    return INDEX_NONE;
}

// for handling cases of swapping 2 occupied indices
void UInventoryComponent::SwapInventoryItems(const int32& IndexFrom, const int32& IndexTo)
{
    // only swap the pointers (i.e. 0xa0... and 0xbc...), the content that pointer points to still the same
    if (Items[IndexFrom] && Items[IndexTo])
    {
        SwapWithOccupiedIndex(IndexFrom, IndexTo);
    }
    // since DragDrop (first stage of Swapping, what invokes this function) only called when the `From` Item != nullptr link:InventorySlot_W.cpp:"void UInventorySlot_W::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, OUT UDragDropOperation*& OutOperation)"
    // -> the `From` Item cannot be null, if 1 item is null then it has to be `To` Item
    else
    {
        SwapWithEmptyIndex(IndexFrom, IndexTo);
    }
}

void UInventoryComponent::SwapWithOccupiedIndex(const int32& IndexFrom, const int32& IndexTo)
{
    Items[IndexFrom]->Index1D = IndexTo;
    Items[IndexTo]->Index1D = IndexFrom;
    Items.Swap(IndexFrom, IndexTo);
}

void UInventoryComponent::SwapWithEmptyIndex(const int32& IndexFrom, const int32& IndexTo)
{
    Items[IndexFrom]->Index1D = IndexTo;
    Items.Swap(IndexFrom, IndexTo);
}
