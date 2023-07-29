// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/OptionOfSortedModeButton_WC.h"
#include "Components/VerticalBoxSlot.h"
#include "MyProject/LogUtilities.h"
#include "MyProject/MyProjectCharacter.h"

void UOptionOfSortedModeButton_WC::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    //MyProjectCharacter = Cast<AMyProjectCharacter>(GetOwningPlayerPawn());

    InitOptionOfFindButtonUI();

    // Display Settings
    Border->Visibility = ESlateVisibility::Hidden; // hidden by default
    bIsFocusable = true; // set widget focusable
}

void UOptionOfSortedModeButton_WC::SynchronizeProperties()
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

void UOptionOfSortedModeButton_WC::InitOptionOfFindButtonUI()
{
    if (!DescriptionSlotClass)
    {
        V_LOG("DescriptionSlotClass is NULL");
        return;
    }

    ListOptions->ClearChildren();

    int Index = 0;
    for (int Number = 0; Number < NumberOfButton; Number++)
    {
        const auto OptionSlot_Widget = CreateWidget<UButtonDescription_WC>(this, DescriptionSlotClass);
        OptionSlotSize = OptionSlot_Widget->Dimension;

        //Set Text and Name for all buttonDescription
        if (Index == 0)
        {
            FString Name = "Ascending (A to Z)";
            OptionSlot_Widget->TextBlock->SetText(FText::AsCultureInvariant("Ascending (A to Z)"));
            OptionSlot_Widget->ButtonName = FName((*Name));
            OptionSlot_Widget->Id = ENameSortedModeButton::AToZ;
            Index++;
        }
        else if (Index == 1)
        {
            FString Name = "Price high to low";
            OptionSlot_Widget->TextBlock->SetText((FText::AsCultureInvariant("Price high to low")));
            OptionSlot_Widget->ButtonName = FName((*Name));
            OptionSlot_Widget->Id = ENameSortedModeButton::PriceHighToLow;
            Index++;
        }
        else if (Index == 2)
        {
            FString Name = "Price low to high";
            OptionSlot_Widget->TextBlock->SetText(FText::AsCultureInvariant("Price low to high"));
            OptionSlot_Widget->ButtonName = FName((*Name));
            OptionSlot_Widget->Id = ENameSortedModeButton::PriceLowToHigh;
            Index++;
        }
        else if (Index==3)
        {
            FString Name = "Rarity high to low";
            OptionSlot_Widget->TextBlock->SetText(FText::AsCultureInvariant("Rarity high to low"));
            OptionSlot_Widget->ButtonName = FName((*Name));
            OptionSlot_Widget->Id = ENameSortedModeButton::RarityHighToLow;
            Index++;
        }
        else if (Index==4)
        {
            FString Name = "Rarity low to high";
            OptionSlot_Widget->TextBlock->SetText(FText::AsCultureInvariant("Rarity low to high"));
            OptionSlot_Widget->ButtonName = FName((*Name));
            OptionSlot_Widget->Id = ENameSortedModeButton::RarityLowToHigh;
            Index++;
        }
        else if (Index==5)
        {
            FString Name = "Latest";
            OptionSlot_Widget->TextBlock->SetText(FText::AsCultureInvariant("Latest"));
            OptionSlot_Widget->ButtonName = FName((*Name));
            OptionSlot_Widget->Id = ENameSortedModeButton::Latest;
            Index++;
        }

        if (!IsDesignTime())
        {
            OptionSlot_Widget->OnClickedButtonDescription.AddUniqueDynamic(this, &UOptionOfSortedModeButton_WC::OnClickedButton);
        }

        UVerticalBoxSlot* VerticalBoxSlot = ListOptions->AddChildToVerticalBox(OptionSlot_Widget);
        VerticalBoxSlot->SetVerticalAlignment(VAlign_Center);
    }

}

void UOptionOfSortedModeButton_WC::CalculateWidgetDimension()
{
    // TODO: Get Dimension from UButtonDescription -> OptionSlotSize <haibn>
    Dimension.X = 300;
    Dimension.Y = 64 * NumberOfButton;
}

//link:OptionOfSortedModeButton_WC.cpp:"InitOptionOfFindButtonUI()"
void UOptionOfSortedModeButton_WC::OnClickedButton(const int IdButton)
{
    OnClickedButtonOfSortedMode.Broadcast(IdButton);
}
