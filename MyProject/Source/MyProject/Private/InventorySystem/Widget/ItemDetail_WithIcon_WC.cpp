// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/ItemDetail_WithIcon_WC.h"

#include "Components/HorizontalBoxSlot.h"
#include "MyProject/LogUtilities.h"
//#include "ItemDetail_WithIcon_WC.h"

void UItemDetail_WithIcon_WC::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    //get UI fromBluePrint
    LikeButton = GetLikeButton_WC();
    NameOfItemButton->OnClicked.AddUniqueDynamic(this, &UItemDetail_WithIcon_WC::SendItem);
    //LikeButton = GetLikeButton();
}

void UItemDetail_WithIcon_WC::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    //InitItemDetailWithIcon();
    
    // update visibility of this widget
    this->SetVisibility(ESlateVisibility::Visible);

    // set the display to Desire On Screen
    this->DesignSizeMode = EDesignPreviewSizeMode::DesiredOnScreen;
}

//link:ItemDetail_WithIcon_WC.cpp:"NativeOnInitialized()"
void UItemDetail_WithIcon_WC::SendItem()
{
    if(!Item)
    {
        V_LOG("Item is NULL");
        return;
    }
    SendClickedItem.Broadcast(Item);
}
