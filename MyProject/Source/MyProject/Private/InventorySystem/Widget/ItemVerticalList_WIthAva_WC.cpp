// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/ItemVerticalList_WIthAva_WC.h"

//#include "InventorySystem/Common/Structs.h"
//#include <complex.h>

#include "InventorySystem/Item/ItemBase.h"
#include "Components/VerticalBoxSlot.h"
#include "Crafting System/CraftingComponent.h"
#include "MyProject/LogUtilities.h"

void UItemVerticalList_WIthAva_WC::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    MyProjectCharacter = Cast<AMyProjectCharacter>(GetOwningPlayerPawn());

    CraftingComponent = MyProjectCharacter->Crafting;
    Update();
}

void UItemVerticalList_WIthAva_WC::SynchronizeProperties()
{
    Super::SynchronizeProperties();
}

void UItemVerticalList_WIthAva_WC::Update()
{
    if(!ITemDetailWithIconClass)
    {
        V_LOG("ItemDetaiWithIconClass is NULL");
        return;
    }

    VerticalBox->ClearChildren();
    for(const auto &IngredientList : CraftingComponent->IngredientListsData)
    {
        const auto ItemVerticalListSlot = CreateWidget<UItemDetail_WithIcon_WC>(this, ITemDetailWithIconClass);

        //send item when player clicked NameOfItem UI
        ItemVerticalListSlot->Item = IngredientList.Key;
            //V_LOG("KEY=%s=", *ItemVerticalListSlot->Item->DataName.ToString());

        ItemVerticalListSlot->SendClickedItem.AddUniqueDynamic(this, &UItemVerticalList_WIthAva_WC::OnClickedNameOfItem);

        ItemVerticalListSlot->ItemTextName->SetText(FText::FromName(IngredientList.Key->DataName).ToUpper());
        if (CraftingComponent->CraftLists.Contains(IngredientList.Key))
        {
            ItemVerticalListSlot->NotificationText->SetText(FText::AsCultureInvariant("Can Craft"));
        }
        else
        {
            ItemVerticalListSlot->NotificationText->SetText(FText::AsCultureInvariant("Can Not Craft"));
        }

        //add Item into Array when player click like item
        ItemVerticalListSlot->LikeButton->Item = IngredientList.Key;
        ItemVerticalListSlot->LikeButton->OnClickedLikeButton.AddUniqueDynamic(this, &UItemVerticalList_WIthAva_WC::AddItemToList);

        UVerticalBoxSlot* VerticalBoxSlot = VerticalBox->AddChildToVerticalBox(ItemVerticalListSlot);
        VerticalBoxSlot->SetVerticalAlignment(VAlign_Center);
        
    }
}

void UItemVerticalList_WIthAva_WC::ShowItemCanCraft()
{
    if (!ITemDetailWithIconClass)
    {
        V_LOG("ItemDetaiWithIconClass is NULL");
        return;
    }

    VerticalBox->ClearChildren();
    for (const auto& ItemCanCraft : CraftingComponent->CraftLists)
    {
        const auto ItemVerticalListSlot = CreateWidget<UItemDetail_WithIcon_WC>(this, ITemDetailWithIconClass);

        //send item when player clicked NameOfItem UI
        ItemVerticalListSlot->Item = ItemCanCraft.Key;
        ItemVerticalListSlot->SendClickedItem.AddUniqueDynamic(this, &UItemVerticalList_WIthAva_WC::OnClickedNameOfItem);

        ItemVerticalListSlot->ItemTextName->SetText(FText::FromName(ItemCanCraft.Key->DataName).ToUpper());
        ItemVerticalListSlot->NotificationText->SetText(FText::AsCultureInvariant("Can Craft"));

        UVerticalBoxSlot* VerticalBoxSlot = VerticalBox->AddChildToVerticalBox(ItemVerticalListSlot);
        VerticalBoxSlot->SetVerticalAlignment(VAlign_Center);

        //add Item into Array when player click like item
        ItemVerticalListSlot->LikeButton->Item = ItemCanCraft.Key;
        ItemVerticalListSlot->LikeButton->OnClickedLikeButton.AddUniqueDynamic(this, &UItemVerticalList_WIthAva_WC::AddItemToList);
    }
}

void UItemVerticalList_WIthAva_WC::ShowLikedItem()
{
    if (!ITemDetailWithIconClass)
    {
        V_LOG("ItemDetaiWithIconClass is NULL");
        return;
    }

    VerticalBox->ClearChildren();
    for (const auto& LikedItem : CraftingComponent->ListFavoritesItems)
    {
        const auto ItemVerticalListSlot = CreateWidget<UItemDetail_WithIcon_WC>(this, ITemDetailWithIconClass);

        //send item when player clicked NameOfItem UI
        ItemVerticalListSlot->Item = LikedItem;
        ItemVerticalListSlot->SendClickedItem.AddUniqueDynamic(this, &UItemVerticalList_WIthAva_WC::OnClickedNameOfItem);

        ItemVerticalListSlot->ItemTextName->SetText(FText::FromName(LikedItem->DataName).ToUpper());

        if(CraftingComponent->CraftLists.Find(LikedItem))
        {
            ItemVerticalListSlot->NotificationText->SetText(FText::AsCultureInvariant("Can Craft"));
        }
        else
        {
            ItemVerticalListSlot->NotificationText->SetText(FText::AsCultureInvariant("Can not Craft"));
        }

        UVerticalBoxSlot* VerticalBoxSlot = VerticalBox->AddChildToVerticalBox(ItemVerticalListSlot);
        VerticalBoxSlot->SetVerticalAlignment(VAlign_Center);

        //add Item into Array when player click like item
        ItemVerticalListSlot->LikeButton->Item = LikedItem;
        ItemVerticalListSlot->LikeButton->OnClickedLikeButton.AddUniqueDynamic(this, &UItemVerticalList_WIthAva_WC::AddItemToList);
    }
}

void UItemVerticalList_WIthAva_WC::ShowFoundItems()
{
    if (!ITemDetailWithIconClass)
    {
        V_LOG("ItemDetaiWithIconClass is NULL");
        return;
    }

    VerticalBox->ClearChildren();
    for (const auto& LikedItem : CraftingComponent->ResultItemsListAfterSearching)
    {
        const auto ItemVerticalListSlot = CreateWidget<UItemDetail_WithIcon_WC>(this, ITemDetailWithIconClass);

        //send item when player clicked NameOfItem UI
        ItemVerticalListSlot->Item = LikedItem;
        ItemVerticalListSlot->SendClickedItem.AddUniqueDynamic(this, &UItemVerticalList_WIthAva_WC::OnClickedNameOfItem);

        ItemVerticalListSlot->ItemTextName->SetText(FText::FromName(LikedItem->DataName).ToUpper());

        if (CraftingComponent->CraftLists.Find(LikedItem))
        {
            ItemVerticalListSlot->NotificationText->SetText(FText::AsCultureInvariant("Can Craft"));
        }
        else
        {
            ItemVerticalListSlot->NotificationText->SetText(FText::AsCultureInvariant("Can not Craft"));
        }

        UVerticalBoxSlot* VerticalBoxSlot = VerticalBox->AddChildToVerticalBox(ItemVerticalListSlot);
        VerticalBoxSlot->SetVerticalAlignment(VAlign_Center);

        //add Item into Array when player click like item
        ItemVerticalListSlot->LikeButton->Item = LikedItem;
        ItemVerticalListSlot->LikeButton->OnClickedLikeButton.AddUniqueDynamic(this, &UItemVerticalList_WIthAva_WC::AddItemToList);
    }
}

void UItemVerticalList_WIthAva_WC::AddItemToList(AItemBase* ClickedItem)
{
    if(!CraftingComponent->ListFavoritesItems.Contains(ClickedItem))
    {
        CraftingComponent->ListFavoritesItems.Add(ClickedItem);
    }

    //when the player clicked the same button twice => remove ClickedItem 
    if(!CountNumberOfClickedItem.Contains(ClickedItem))
    {
        CountNumberOfClickedItem.Add(ClickedItem,1);
    }
    else
    {
        CraftingComponent->ListFavoritesItems.Remove(ClickedItem);
        CountNumberOfClickedItem.Remove(ClickedItem);
        ShowLikedItem();
    }
}

void UItemVerticalList_WIthAva_WC::OnClickedNameOfItem(AItemBase* ItemData)
{
    OnClickedNameOfItemButton.Broadcast(ItemData);
}

//TODO: Think Optimization this because they all use the same array (CraftingComponent->IngredientListsData) <haibn>
void UItemVerticalList_WIthAva_WC::ShowItemAToZ()
{
    VerticalBox->ClearChildren();
    CraftingComponent->IngredientListsData.Sort(FSortAToZ());
    Update();
}

void UItemVerticalList_WIthAva_WC::ShowItemPriceHighToLow()
{
    VerticalBox->ClearChildren();
    CraftingComponent->IngredientListsData.Sort(FPriceHighToLow());
    Update();
}

void UItemVerticalList_WIthAva_WC::ShowItemPriceLowToHigh()
{
    VerticalBox->ClearChildren();
    CraftingComponent->IngredientListsData.Sort(FPriceLowToHigh());
    Update();
}

void UItemVerticalList_WIthAva_WC::ShowItemLatest()
{
    VerticalBox->ClearChildren();
    CraftingComponent->IngredientListsData.Sort(FLatest());
    Update();
}

void UItemVerticalList_WIthAva_WC::ShowItemRarityLowToHigh()
{
    VerticalBox->ClearChildren();
    CraftingComponent->IngredientListsData.Sort(FRarityLowToHigh());
    Update();
}

void UItemVerticalList_WIthAva_WC::ShowItemRarityHighToLow()
{
    VerticalBox->ClearChildren();
    CraftingComponent->IngredientListsData.Sort(FRarityHighToLow());
    Update();
}



