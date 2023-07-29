// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/ActionBar_WC.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/UniformGridSlot.h"
#include "InventorySystem/WidgetDrag.h"
#include "MyProject/LogUtilities.h"

#define OUT

void UActionBar_WC::NativeOnInitialized()
{
    UE_LOG(LogTemp, Warning, TEXT("UActionBar_WC::UActionBar_WC::NativeOnInitialized() is called"));
    Super::NativeOnInitialized();

    // bind Lock Event to `Checkbox`
    TScriptDelegate<FWeakObjectPtr> LockBar_0;
    LockBar_0.BindUFunction(this, "LockBar");
    CheckBox->OnCheckStateChanged.Add(LockBar_0);

    // set the widget focusable
    bIsFocusable = true;
}

void UActionBar_WC::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    // init the action bar
    RefreshActionBar();

    // safe get the size of action slot
    if (const auto ActionSlot = Cast<UActionSlot_WC>(UniformGridPanel->GetChildAt(0)))
    {
        ActionSlotSize = ActionSlot->Dimension;
    }
    CalculateWidgetDimension();

    // set the proper width for SizeBox
    SizeBox->SetHeightOverride(Dimension.Y);
    SizeBox->SetWidthOverride(Dimension.X);

    // update visibility of this widget
    this->SetVisibility(ESlateVisibility::Visible);

    // set the display to Desire On Screen
    //this->DesignSizeMode = EDesignPreviewSizeMode::DesiredOnScreen;

    // get the total number of actions for index assignment in the for reference
    NumOfActions = UniformGridPanelMaxRow * UniformGridPanelMaxCol;
}

FReply UActionBar_WC::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    auto Default = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    // don't do anything if bar in Locked
    if (IsBarLocked == true)
    {
        return Default;
    }

    // Bar in drag-able mode	
    const auto AbsoluteCoord = InMouseEvent.GetScreenSpacePosition();
    Offset = InGeometry.AbsoluteToLocal(AbsoluteCoord); //  relative to local space only

    // reply to `Mouse Button Down` Event with drag detection (so enter drag mode when mouse button down)
    const auto EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

    return EventReply.NativeReply;
}

void UActionBar_WC::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
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

void UActionBar_WC::LockBar()
{
    IsBarLocked = !IsBarLocked;
}

bool UActionBar_WC::RefreshActionBar()
{
    // null check 'cause `ActionSlotClass` might not have been initialized at Editor time
    if (!ActionSlotClass)
    {
        return false;
    }

    // create the slots inside the inventory widget
    for (int row = 0; row < UniformGridPanelMaxRow; row++)
    {
        for (int col = 0; col < UniformGridPanelMaxCol; col++)
        {
            const auto ActionSlot = CreateWidget<UActionSlot_WC>(this, ActionSlotClass);
            ActionSlot->ActionSlotIndex = row * UniformGridPanelMaxCol + col;
            UUniformGridSlot* GridSlot = UniformGridPanel->AddChildToUniformGrid(ActionSlot, row, col);
            GridSlot->SetVerticalAlignment(VAlign_Center);
            GridSlot->SetHorizontalAlignment(HAlign_Center);
        }
    }

    return true;
}

void UActionBar_WC::CalculateWidgetDimension()
{
    Dimension.X = UniformGridPanelMaxCol * (ActionSlotSize.X + UniformGridPanel->SlotPadding.Left + UniformGridPanel->
        SlotPadding.Right) + SizeBoxOverrideExtraSpace.Horizontal;
    Dimension.Y = UniformGridPanelMaxRow * (ActionSlotSize.Y + UniformGridPanel->SlotPadding.Top + UniformGridPanel->
        SlotPadding.Bottom) + SizeBoxOverrideExtraSpace.Vertical;
        UE_LOG(LogTemp, Warning, TEXT("The integer value of dimension is: %d"), Dimension.X);
        UE_LOG(LogTemp, Warning, TEXT("The integer value of dimension is: %d"), Dimension.Y);
}
