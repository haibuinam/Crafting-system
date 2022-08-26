// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/Widget/HUD_WC.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "GameFramework/HUD.h"
#include "InventorySystem/WidgetDrag.h"

#include "MyProject/MyProjectCharacter.h"
#include "MyProject/LogUtilities.h"

void UHUD_WC::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    this->SetVisibility(ESlateVisibility::Visible);

    InitHUD();

    // set this widget to be focusable
    bIsFocusable = true;

    // set the init state of the background blur to not visible
    BackgroundBlur->Visibility = ESlateVisibility::Hidden;
}

void UHUD_WC::SynchronizeProperties()
{
    Super::SynchronizeProperties();


    InitHUD();
}

bool UHUD_WC::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                           UDragDropOperation* InOperation)
{
    // get drag operation info in full data (by casting to our specific class)
    const auto WidgetDragOperation = Cast<UWidgetDrag>(InOperation);
    if (!WidgetDragOperation)
    {
        V_LOG(
            "Object not derived from UWidgetDrag. Could be from UItemDrag. Double-check the return type/class of `OutOperation` in OnDragDetected Event of this Widget");
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

void UHUD_WC::InitHUD()
{
    // ref the UI elements
    InventoryWidget = GetInventoryWidget();

    ActionBarWidget = GetActionBarWidget();

    HUDCraftingWidget = GetHUDCraftingWidget();
}


void UHUD_WC::ShowOptionOfFindButton()
{
    UE_LOG(LogTemp, Warning, TEXT("HUD is Running"));
}


