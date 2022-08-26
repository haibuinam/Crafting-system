// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/ItemDescription_WC.h"

#include "InventorySystem/Item/ItemBase.h"
#include "InventorySystem/Item/WeaponItem.h"
#include "InventorySystem/Item/FoodItem.h"
#include "MyProject/LogUtilities.h"

void UItemDescription_WC::Update(AItemBase* Item)
{
    if(!Item)
    {
        V_LOG("Item is Null");
        return;
    }

    NameOfItem->SetText(FText::FromName(Item->DataName).ToUpper());
    ItemRarity->SetText(FText::AsCultureInvariant(Item->Rarity));
    Description->SetText(FText::AsCultureInvariant(Item->Description));
    ItemStyle->SetText(FText::AsCultureInvariant(Item->Type));
    DetailItem->SetText(FText::AsCultureInvariant(Item->Uses));
    ItemUses->SetText(FText::AsCultureInvariant(Item->UseActionText));
}
