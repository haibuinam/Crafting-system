// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/HUDCrafting_WC.h"

#include "MyProject/MyProjectCharacter.h"
#include "Crafting System/CraftingComponent.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
//#include "MyProject/MyProjectCharacter.h"
#include <string>

#include "ImageUtils.h"
#include "InventorySystem/WidgetDrag.h"
#include "InventorySystem/Item/ItemBase.h"
#include "MyProject/LogUtilities.h"
//#include "MyProject/LogUtilities.h"
//#include <Runtime/Engine/Private/ImageUtils.cpp>
//#include "HUDCrafting_WC.h"
//#include "HUDCrafting_WC.h"

void UHUDCrafting_WC::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    MyPlayerController = Cast<AMyPlayerController>(GetOwningPlayer());

    MyProjectCharacter = Cast<AMyProjectCharacter>(GetOwningPlayerPawn());

    CraftingComponent = MyProjectCharacter->Crafting;

    InitHUDCraftingWidget();
    ExitButton->OnClicked.AddUniqueDynamic(this, &UHUDCrafting_WC::CloseHUDCrafting);

    //Hidden by default
    Border->SetVisibility(ESlateVisibility::Hidden);
    bIsFocusable = true;
}

void UHUDCrafting_WC::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    //InitHUDCraftingWidget();
    this->SetVisibility(ESlateVisibility::Visible);
}


void UHUDCrafting_WC::InitHUDCraftingWidget()
{
    //set image default for ImageCraftingItemDisplay
    const FString PathImage = "ImageCrafting/";
    FString FullPath = FPaths::ProjectContentDir() + PathImage + "anvil.png";
    UTexture2D* DefaultIcon2D = FImageUtils::ImportFileAsTexture2D(FullPath);
    ImageCraftingItemDisplay->SetBrushFromTexture(DefaultIcon2D);

    // ref the UI elements
    CraftingCircle_With8Item_CraftingWidget = GetCraftingCircleWidget();

    OptionOfVerticalListWidget = GetOptionOfVerticalListWidget();
    OptionOfVerticalListWidget->OnClickedButtonOfVerticalList.AddUniqueDynamic(this, &UHUDCrafting_WC::OnClickedButtonsOfOptionOfVerticalListWidget);

    BarComponentCrafting = GetBarComponentCraftingWidget();
    BarComponentCrafting->OnClickedBarComponentButton.AddUniqueDynamic(this, &UHUDCrafting_WC::OnClickedBarComponentButton);

    ItemVerticalListWIthAva = GetUItemVerticalListWIthAvaWidget();
    ItemVerticalListWIthAva->OnClickedNameOfItemButton.AddUniqueDynamic(this, &UHUDCrafting_WC::OnClickedNameOfItemVerticalListWithAva);

    ItemDescriptionCrafting = GetUItemDescriptionWidget();

    OptionOfFindButton = GetOptionOfFindButtonWidget();
    OptionOfFindButton->OnClickedButtonOfFindButton.AddUniqueDynamic(this, &UHUDCrafting_WC::OnClickedButtonOfFindButtonWC);

    OptionOfSortedModeButton = GetOptionOfSortedModeButtonWidget();
    OptionOfSortedModeButton->OnClickedButtonOfSortedMode.AddUniqueDynamic(this, &UHUDCrafting_WC::OnClickedButtonOfOptionOfSortedModeButton);

    QuantityAndTotalAmountWidget = GetQuantityAndTotalAmountWidget();
    QuantityAndTotalAmountWidget->OnClickedForgingButton.AddUniqueDynamic(this, &UHUDCrafting_WC::OnClickedForgingButton);
}

// Native coppy from HUD_WC
bool UHUDCrafting_WC::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    // get drag operation info in full data (by casting to our specific class)
    const auto WidgetDragOperation = Cast<UWidgetDrag>(InOperation);
    if (!WidgetDragOperation)
    {
        V_LOG(
            "Messages from UHUDCrafting_WC: Object not derived from UWidgetDrag. Could be from UItemDrag. Double-check the return type/class of `OutOperation` in OnDragDetected Event of this Widget");
        return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
    }

    // calculate new position
    const FVector2D DropWindowOffset = WidgetDragOperation->DropWindowOffset;
    const FVector2D CurrentWorldPosition = InDragDropEvent.GetScreenSpacePosition();
    const FVector2D CurrentLocalPosition = InGeometry.AbsoluteToLocal(CurrentWorldPosition);
    const FVector2D NewLocalPosition = CurrentLocalPosition - DropWindowOffset;

    UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop: DropWindowOffset: %s"), *DropWindowOffset.ToString());

    // get the current widget we are dragging from the `InOperation`
    // (which always store information about current Drag Operation: including dragging object, screen space, etc..)
    UUserWidget* WidgetToDrag = WidgetDragOperation->WidgetToDrag;
    // add the widget being dragged to viewport
    WidgetToDrag->AddToViewport();
    // set position of the widget being dragged to new position
    WidgetToDrag->SetPositionInViewport(NewLocalPosition, false);

    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UHUDCrafting_WC::OnClickedButtonOfFindButtonWC(const int IdButtonOfFindButtonWC)
{
    if(IdButtonOfFindButtonWC == ENameOptionOfFindButton::CanCraft)
    {
        ItemVerticalListWIthAva->ShowItemCanCraft();
    }
    else if(IdButtonOfFindButtonWC == ENameOptionOfFindButton::All)
    {
        ItemVerticalListWIthAva->Update();
    }
    else if (IdButtonOfFindButtonWC == ENameOptionOfFindButton::Like)
    {
        ItemVerticalListWIthAva->ShowLikedItem();
    }
}

//when player clicked Item of VerticalListWithAva -> update ItemDescription (show info item)
//and update Circle Crafting.
void UHUDCrafting_WC::OnClickedNameOfItemVerticalListWithAva(AItemBase* ItemClicked)
{
    if (!ItemClicked)
    {
        V_LOG("Item is Null");
        return;
    }
    
    ItemDescriptionCrafting->Update(ItemClicked);
    CraftingCircle_With8Item_CraftingWidget->Update(ItemClicked);

    //TODO: try using another way <haibn>
    //ref Item to QuantityAndTotalAmount
    //QuantityAndTotalAmountWidget->SelectedItem = ItemClicked;
    QuantityAndTotalAmountWidget->GetSelectedItem(ItemClicked);
    //AItemBase* temp = ItemClicked;
}

void UHUDCrafting_WC::OnClickedButtonOfOptionOfSortedModeButton(const int IdButtonOfOptionSortedMode)
{
    //cannot show items if it haven't Ingredient List
    if(IdButtonOfOptionSortedMode == ENameSortedModeButton::AToZ)
    {
        ItemVerticalListWIthAva->ShowItemAToZ();
    }
    else if (IdButtonOfOptionSortedMode == ENameSortedModeButton::PriceHighToLow)
    {
        ItemVerticalListWIthAva->ShowItemPriceHighToLow();
    }
    else if (IdButtonOfOptionSortedMode == ENameSortedModeButton::PriceLowToHigh)
    {
        ItemVerticalListWIthAva->ShowItemPriceLowToHigh();
    }
    else if (IdButtonOfOptionSortedMode == ENameSortedModeButton::Latest)
    {
        ItemVerticalListWIthAva->ShowItemLatest();
    }
    else if (IdButtonOfOptionSortedMode == ENameSortedModeButton::RarityHighToLow)
    {
        ItemVerticalListWIthAva->ShowItemRarityHighToLow();
    }
    else if (IdButtonOfOptionSortedMode == ENameSortedModeButton::RarityLowToHigh)
    {
        ItemVerticalListWIthAva->ShowItemRarityLowToHigh();
    }
}

void UHUDCrafting_WC::OnClickedForgingButton(AItemBase* SelectedItem, const int NumberItemPlayerWantToMake)
{
    CraftingComponent->CraftingManager(SelectedItem, NumberItemPlayerWantToMake);
    CraftingCircle_With8Item_CraftingWidget->Update(SelectedItem);

    //ItemVerticalListWIthAva->ShowItemCanCraft();
    //QuantityAndTotalAmountWidget->

}

void UHUDCrafting_WC::OnClickedBarComponentButton(const int IdBarButtonWidget)
{
    //ToDo: create widget for Bar (To send image when player clicked button -> update ImageCraftingItemDisplay <haibn>
    const FString PathImage = "ImageCrafting/";
    if(IdBarButtonWidget == ENameBarComponentCrafting::CraftingUI)
    {
        CraftingCanvasPanel->SetVisibility(ESlateVisibility::Visible);
        FString FullPath = FPaths::ProjectContentDir() + PathImage + "anvil.png";
        UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
        ImageCraftingItemDisplay->SetBrushFromTexture(Texture);
    }
    else if (IdBarButtonWidget == ENameBarComponentCrafting::EnhanceUI)
    {
        CraftingCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
        FString FullPath = FPaths::ProjectContentDir() + PathImage + "hammer.png";
        UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
        ImageCraftingItemDisplay->SetBrushFromTexture(Texture);
    }
    //TODO: continue enhance <haibn>
}

void UHUDCrafting_WC::CloseHUDCrafting()
{
    this->Border->SetVisibility(ESlateVisibility::Hidden);
}

void UHUDCrafting_WC::OnClickedButtonsOfOptionOfVerticalListWidget(const int IdButtonOfOptionVerticalList)
{
    if(IdButtonOfOptionVerticalList == ENameOptionOfVerticalListButton::FindButton)
    {
        V_LOG("FindButton Of OptionOfVertical is clicked");
        ItemVerticalListWIthAva->ShowFoundItems();
    }
    else if(IdButtonOfOptionVerticalList == ENameOptionOfVerticalListButton::ModeButton &&!IsShowOptionModeButton)
    {
        OptionOfFindButton->Border->SetVisibility(ESlateVisibility::Visible);
        IsShowOptionModeButton = true;
    }
    else if (IdButtonOfOptionVerticalList == ENameOptionOfVerticalListButton::ModeButton && IsShowOptionModeButton)
    {
        OptionOfFindButton->Border->SetVisibility(ESlateVisibility::Hidden);
        IsShowOptionModeButton = false;
    }
    else if(IdButtonOfOptionVerticalList == ENameOptionOfVerticalListButton::SortedMode && !IsShowOptionOfSortedModeButton)
    {
        OptionOfSortedModeButton->Border->SetVisibility(ESlateVisibility::Visible);
        IsShowOptionOfSortedModeButton = true;
    }
    else if (IdButtonOfOptionVerticalList == ENameOptionOfVerticalListButton::SortedMode && IsShowOptionOfSortedModeButton)
    {
        OptionOfSortedModeButton->Border->SetVisibility(ESlateVisibility::Hidden);
        IsShowOptionOfSortedModeButton = false;
    }
}
