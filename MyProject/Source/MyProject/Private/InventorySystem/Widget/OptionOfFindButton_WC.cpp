// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/OptionOfFindButton_WC.h"
#include "InventorySystem/Widget/ButtonDescription_WC.h"
#include "Components/VerticalBoxSlot.h"
#include "MyProject/LogUtilities.h"
#include "MyProject/MyProjectCharacter.h"

void UOptionOfFindButton_WC::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    MyProjectCharacter = Cast<AMyProjectCharacter>(GetOwningPlayerPawn());

    CraftingComponent = MyProjectCharacter->Crafting;
   

    InitOptionOfFindButtonUI();

    // Display Settings
    Border->Visibility = ESlateVisibility::Hidden; // hidden by default
    bIsFocusable = true; // set widget focusable
}

void UOptionOfFindButton_WC::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    if (IsDesignTime())
    {
        InitOptionOfFindButtonUI();

        CalculateWidgetDimension();

        // set the proper width for SizeBox
        SizeBox->SetHeightOverride(Dimension.Y);
        SizeBox->SetWidthOverride(Dimension.X);

        // update visibility of this widget
        //this->SetVisibility(ESlateVisibility::Visible);

        // set the display to Desire On Screen
        //this->DesignSizeMode = EDesignPreviewSizeMode::DesiredOnScreen;
    }
    
}

void UOptionOfFindButton_WC::InitOptionOfFindButtonUI()
{
    if(!DescriptionSlotClass)
    {
        return;
    }

    ListOptions->ClearChildren();

    int Index = 0;
    for( int Number = 0; Number<NumberOfButton; Number++)
    {
        const auto OptionSlot_Widget = CreateWidget<UButtonDescription_WC>(this, DescriptionSlotClass);
        OptionSlotSize = OptionSlot_Widget->Dimension;

        //UE_LOG(LogTemp, Warning, TEXT("CraftingSlot X : %f"), OptionSlot_Widget->Dimension.X);
        //UE_LOG(LogTemp, Warning, TEXT("CraftingSlot Y : %f"), OptionSlot_Widget->Dimension.Y);

        //Set Text for all buttonDescription
        if(Index==0)
        {
            OptionSlot_Widget->TextBlock->SetText(FText::AsCultureInvariant("Can Craft"));
            //OptionSlot_Widget->ButtonName = FName(TEXT("Can Craft"));
            OptionSlot_Widget->Id = ENameOptionOfFindButton::CanCraft;
            Index++;
        }
        else if(Index==1)
        {
            OptionSlot_Widget->TextBlock->SetText((FText::AsCultureInvariant("All")));
            //OptionSlot_Widget->ButtonName = FName(TEXT("All"));
            OptionSlot_Widget->Id = ENameOptionOfFindButton::All;
            Index++;
        }
        else if(Index==2)
        {
            OptionSlot_Widget->TextBlock->SetText(FText::AsCultureInvariant("Like"));
            //OptionSlot_Widget->ButtonName = FName(TEXT("Like"));
            OptionSlot_Widget->Id = ENameOptionOfFindButton::Like;
            Index++;
        }

        if(!IsDesignTime())
        {
            OptionSlot_Widget->OnClickedButtonDescription.AddUniqueDynamic(this, &UOptionOfFindButton_WC::OnClickedButton);
        }

        //add to UI
        UVerticalBoxSlot* VerticalBoxSlot = ListOptions->AddChildToVerticalBox(OptionSlot_Widget);
        VerticalBoxSlot->SetVerticalAlignment(VAlign_Center);
    }

}

void UOptionOfFindButton_WC::CalculateWidgetDimension()
{
    // TODO: Get Dimension from UButtonDescription -> OptionSlotSize
    Dimension.X = 194;
    Dimension.Y = 64 * NumberOfButton;

}

//link:OptionOfFindButton_WC.cpp:"InitOptionOfFindButtonUI()"
void UOptionOfFindButton_WC::OnClickedButton(const int IdButton)
{
    OnClickedButtonOfFindButton.Broadcast(IdButton);
    //V_LOG("BUTTON NAME %s", *NameButton.ToString());
}

