// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/Circle_With8Item_Crafting_WC.h"

#include <string>

#include "ImageUtils.h"
#include "Components/CanvasPanelSlot.h"
#include "MyProject/MyProjectCharacter.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridSlot.h"
#include "InventorySystem/ItemDrag.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Crafting System/CraftingComponent.h"
#include "InventorySystem/Widget/Inventory_W.h"
#include "Engine/Canvas.h"
#include "InventorySystem/WidgetDrag.h"
#include "MyProject/LogUtilities.h"
//#include "Circle_With8Item_Crafting_WC.h"

#define OUT


void UCircle_With8Item_Crafting_WC::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    MyProjectCharacter = Cast<AMyProjectCharacter>(GetOwningPlayerPawn());

    CraftingComponent = MyProjectCharacter->Crafting;
    Inventory = MyProjectCharacter->Inventory;

    InitCraftingWidget();

    bIsFocusable = true;

    ItemDescriptionWidget = GetItemDescriptionWidget();
    ItemDescriptionWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UCircle_With8Item_Crafting_WC::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    if (IsDesignTime())
    {
    ItemDescriptionWidget = GetItemDescriptionWidget();
        InitCraftingWidget();

        CalculateWidgetDimension();

        // set the proper width for SizeBox
        SizeBox->SetHeightOverride(Dimension.Y);
        SizeBox->SetWidthOverride(Dimension.X);

        // update visibility of this widget
        this->SetVisibility(ESlateVisibility::Visible);

        // set the display to Desire On Screen
        //this->DesignSizeMode = EDesignPreviewSizeMode::DesiredOnScreen;
    }
}

void UCircle_With8Item_Crafting_WC::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
    OUT UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

    // create new widget drag operation
    UWidgetDrag* WidgetDragOperation = NewObject<UWidgetDrag>();
    WidgetDragOperation->WidgetToDrag = this;
    WidgetDragOperation->DefaultDragVisual = this;
    WidgetDragOperation->DropWindowOffset = Offset;
    WidgetDragOperation->Pivot = EDragPivot::MouseDown;
    V_LOG("Offset: %s", *Offset.ToString());

    // remove old action bar (this one, which is the one still at old position)
    this->RemoveFromParent();

    // this `OutOperation` will be fed into other DragDetected function as `InOperation`
    OUT OutOperation = WidgetDragOperation;
}

bool UCircle_With8Item_Crafting_WC::InitCraftingWidget()
{
    // null check 'cause `CraftingSlotClass` might not have been initialized at Editor time
    if (!CraftingSlotClass)
    {
        return false;
    }

    GetDefaultIcon();
    CircleCanvasPanel->ClearChildren();
    CraftingSlot_Widgets.Empty();

    const FString PathImage = "ImageCrafting/";
    FString FullPath = FPaths::ProjectContentDir() + PathImage + "circle 6.png";
    UTexture2D* CenterDefaultIcon2D = FImageUtils::ImportFileAsTexture2D(FullPath);
    if (!CenterDefaultIcon2D)
    {
        V_LOG("Default Icon 2D is nullptr");
    }
    ImageCenterCraftingSlot->SetBrushFromTexture(CenterDefaultIcon2D);

    int Id = 0;
    // create the slots circle widget
    for (double theta = 0; theta < 360; theta += 360.0/NumberOfItems)
    {
        //set location based on R and theta
        Location.X = R * cos(theta*PI/180.0);
        Location.Y = R * sin(theta*PI/180.0);

        const auto CraftingSlot = CreateWidget<UItemCraftingSlot_WC>(this, CraftingSlotClass);

        CraftingSlot->ItemAmount->SetVisibility(ESlateVisibility::Hidden);
        CraftingSlot->LocationVector2DWidget = Location;
        CraftingSlotSize = CraftingSlot->Dimension;
        
        //add Index widget.
        CraftingSlot->IndexInCrafting = Id;
        Id++;

        if(!IsDesignTime())
        {
                CraftingSlot->OnCraftingAddWidget.AddUniqueDynamic(this, &UCircle_With8Item_Crafting_WC::AddSlotWidgetsFromInventory);
        }

        UCanvasPanelSlot* CanvasPanelSlot = CircleCanvasPanel->AddChildToCanvas(CraftingSlot);
        CanvasPanelSlot->SetAnchors(FAnchors(0.5 ,0.5));
        CanvasPanelSlot->SetAlignment(FVector2D(0.5,0.5));
        CanvasPanelSlot->SetPosition(Location);

        CraftingSlot_Widgets.Add(CanvasPanelSlot);
    }

    return true;
}

void UCircle_With8Item_Crafting_WC::GetDefaultIcon()
{
    const FString PathImage = "ImageCrafting/";
    FString FullPath = FPaths::ProjectContentDir() + PathImage + "black circle.png";
    DefaultIcon2D = FImageUtils::ImportFileAsTexture2D(FullPath);
    if(!DefaultIcon2D)
    {
        V_LOG("Default Icon 2D is nullptr");
    }
}

void UCircle_With8Item_Crafting_WC::CalculateWidgetDimension()
{
    Dimension.X = 500;
    Dimension.Y = 500;
}

void UCircle_With8Item_Crafting_WC::AddSlotWidgetsFromInventory(UItemDrag* WidgetFromInventory, const int32& IndexTo)
{
    const auto ItemDragWidget_From = Cast<UItemDrag>(WidgetFromInventory);
    const auto CanvasPanelSlot_To = CraftingSlot_Widgets[IndexTo];

    const auto CanvasPanelSlot_W_To = Cast<UItemCraftingSlot_WC>(CanvasPanelSlot_To->Content);
    
    if(WidgetFromInventory ==nullptr)
    {
        V_LOG("WidgetFromInventory is NULL");
        return;
    }
    if(ItemDragWidget_From==nullptr||CanvasPanelSlot_W_To==nullptr)
    {
        V_LOG("Casting failed");
        return;
    }

    //get Item data
    CanvasPanelSlot_W_To->Item = ItemDragWidget_From->DraggedItem;

    //set Image of Widget indexTo
    CanvasPanelSlot_W_To->ItemIcon->SetBrushFromTexture(ItemDragWidget_From->ItemImage);

    //set amount
    CanvasPanelSlot_W_To->ItemAmount->SetText(FText::FromString("x" + FString::FromInt(ItemDragWidget_From->DraggedItem->Amount)));
    if (ItemDragWidget_From->DraggedItem->Amount > 1)
    {
        CanvasPanelSlot_W_To->ItemAmount->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        CanvasPanelSlot_W_To->ItemAmount->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UCircle_With8Item_Crafting_WC::Update(AItemBase* ItemUpdate)
{
    if (!ItemUpdate->Thumbnail)
    {
        V_LOG("Thumbnail is NULL");
        return;
    }

    for(const auto &Item : CraftingComponent->IngredientListsData)
    {
        if(Item.Key == ItemUpdate)
        {
            ImageCenterCraftingSlot->SetBrushFromTexture(ItemUpdate->Thumbnail);
            UpdateComponent(Item.Value);

            //An item have only one IngredientList. 
            break;
        }
    }
}

void UCircle_With8Item_Crafting_WC::UpdateComponent(const TSet<AItemBase*>& ItemElement)
{
    //Copy Item From Item Element
    TArray<AItemBase*> ElementsArray = ItemElement.Array();

    //if item have one, two, three -> eight elements
    for(int32 Index = 0 ; Index != CraftingSlot_Widgets.Num(); Index++)
    {
        const auto CanvasPanelSlot = CraftingSlot_Widgets[Index];
        auto Item_W = Cast<UItemCraftingSlot_WC>(CanvasPanelSlot->Content);

        if (Index==2 && !ElementsArray.IsEmpty() && ItemElement.Num() == 1)
        {
            auto ItemPop = ElementsArray.Pop();
            UpdateSlot(Item_W, ItemPop);
        }
        else if (Index > 0 && Index < 3 && !ElementsArray.IsEmpty() && ItemElement.Num() == 2)
        {
            auto ItemPop = ElementsArray.Pop();
            UpdateSlot(Item_W, ItemPop);
        }
        else if(Index>0&&Index<4&&!ElementsArray.IsEmpty()&&ItemElement.Num()==3)
        {
            auto ItemPop = ElementsArray.Pop();
            UpdateSlot(Item_W, ItemPop);
        }
        else if((Index==0||Index<4)&&!ElementsArray.IsEmpty()&&ItemElement.Num()==4)
        {
            auto ItemPop = ElementsArray.Pop();
            UpdateSlot(Item_W, ItemPop);
        }
        else if((Index==0||Index<5)&&!ElementsArray.IsEmpty()&&ItemElement.Num()==5)
        {
            auto ItemPop = ElementsArray.Pop();
            UpdateSlot(Item_W, ItemPop);
        }
        else if((Index==0||Index<6)&&!ElementsArray.IsEmpty()&&ItemElement.Num()==6)
        {
            auto ItemPop = ElementsArray.Pop();
            UpdateSlot(Item_W, ItemPop);
        }
        else if((Index==0||Index<7)&&!ElementsArray.IsEmpty()&&ItemElement.Num()==7)
        {
            auto ItemPop = ElementsArray.Pop();
            UpdateSlot(Item_W, ItemPop);
        }
        else if((Index==0||Index<8)&&!ElementsArray.IsEmpty()&&ItemElement.Num()==8)
        {
            auto ItemPop = ElementsArray.Pop();
            UpdateSlot(Item_W, ItemPop);
        }
        else
        {
            Item_W->ItemIcon->SetBrushFromTexture(DefaultIcon2D);
            Item_W->ItemAmount->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UCircle_With8Item_Crafting_WC::UpdateSlot(UItemCraftingSlot_WC* &Item_W, AItemBase* &ItemPop)
{
    Item_W->ItemIcon->SetBrushFromTexture(ItemPop->Thumbnail);
    Item_W->Item = ItemPop;

    const FString AmountItemAndElement = FString::FromInt(GetAmountOfItemInInventory(Inventory->Items, ItemPop)) +
                                                                    FString(TEXT("/")) + FString::FromInt(ItemPop->Amount);

    //set amount Items Inventory / amount IngredientLists
    Item_W->ItemAmount->SetText(FText::FromString(AmountItemAndElement));
    Item_W->ItemAmount->SetVisibility(ESlateVisibility::Visible);

    Item_W->OnClickedItemCraftingButton.AddUniqueDynamic(this, &UCircle_With8Item_Crafting_WC::ShowItemDescription);
}

void UCircle_With8Item_Crafting_WC::ShowItemDescription(AItemBase* ClickedItem, const FVector2D LocationWidget)
{
    if(IsShowItemDescription)
    {
        ItemDescriptionWidget->SetVisibility(ESlateVisibility::Hidden);
        IsShowItemDescription = false;
        return;
    }

    ItemDescriptionWidget->Update(ClickedItem);

    UCanvasPanelSlot* CanvasPanelSlot = CircleCanvasPanel->AddChildToCanvas(ItemDescriptionWidget);
    CanvasPanelSlot->SetAnchors(FAnchors(0.5 ,0.5));
    CanvasPanelSlot->SetAlignment(FVector2D(0,0));
    CanvasPanelSlot->SetPosition(LocationWidget);
    ItemDescriptionWidget->SetVisibility(ESlateVisibility::Visible);
    IsShowItemDescription = true;
}


int UCircle_With8Item_Crafting_WC::GetAmountOfItemInInventory(const TArray<AItemBase*> &Items, AItemBase* Item)
{
    //because Items of inventory init 20 slots -> game start have 20 slot null. 
    //search by name

    for(int i =0 ;i<Items.Num();i++)
    {
        if (Items[i] != nullptr)
        {
            if(Items[i]->DataName == Item->DataName)
            //if(Items[i] == Item)
            return Items[i]->Amount;
        }
    }
    return 0;
}
