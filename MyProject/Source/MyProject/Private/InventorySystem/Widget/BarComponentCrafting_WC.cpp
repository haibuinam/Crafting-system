// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/BarComponentCrafting_WC.h"

#include "ImageUtils.h"
#include "Components/HorizontalBoxSlot.h"
#include "InventorySystem/Common/Structs.h"
#include "MyProject/LogUtilities.h"
//#include "BarComponentCrafting_WC.h"

void UBarComponentCrafting_WC::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    InitBarUI();
}

void UBarComponentCrafting_WC::SynchronizeProperties()
{
    Super::SynchronizeProperties();
    InitBarUI();
    CalculateWidgetDimension();

    //set the proper width for SizeBox
    SizeBox->SetWidthOverride(Dimension.Y);
    SizeBox->SetHeightOverride(Dimension.X);

    // update visibility of this widget
    this->SetVisibility(ESlateVisibility::Visible);

    // set the display to Desire On Screen
    //this->DesignSizeMode = EDesignPreviewSizeMode::DesiredOnScreen;
}

void UBarComponentCrafting_WC::InitBarUI()
{
    if(!BarSlotClass)
    {
        V_LOG("Bar Slot Class is null");
        return;
    }

    HorizontalBox->ClearChildren();

    const FString PathImage = "ImageCrafting/";

    for(int index=0; index<NumberOfButton; index++)
    {
        const auto ButtonSlot_Widget = CreateWidget<UCraftingButton_WC>(this, BarSlotClass);
        //ButtonSlot_Widget->Id = index;
        ButtonSize = ButtonSlot_Widget->Dimension;

        if(index==0)
        {
            ButtonSlot_Widget->Id = ENameBarComponentCrafting::Button0;
            FString FullPath = FPaths::ProjectContentDir() + PathImage + "sun.png";
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
            ButtonSlot_Widget->ButtonIcon->SetBrushFromTexture(Texture);
        }
        else if(index==1)
        {
            ButtonSlot_Widget->Id = ENameBarComponentCrafting::Button1;
            FString FullPath = FPaths::ProjectContentDir() + PathImage + "fire.png";
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
            ButtonSlot_Widget->ButtonIcon->SetBrushFromTexture(Texture);
        }
        else if(index==2)
        {
            ButtonSlot_Widget->Id = ENameBarComponentCrafting::Button2;
            FString FullPath = FPaths::ProjectContentDir() + PathImage + "pieces.png";
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
            ButtonSlot_Widget->ButtonIcon->SetBrushFromTexture(Texture);
        }
        else if(index==3)
        {
            ButtonSlot_Widget->Id = ENameBarComponentCrafting::CraftingUI;
            FString FullPath = FPaths::ProjectContentDir() + PathImage + "anvil.png";
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
            ButtonSlot_Widget->ButtonIcon->SetBrushFromTexture(Texture);
        }
        else if(index==4)
        {
            ButtonSlot_Widget->Id = ENameBarComponentCrafting::EnhanceUI;
            FString FullPath = FPaths::ProjectContentDir() + PathImage + "hammer.png";
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
            ButtonSlot_Widget->ButtonIcon->SetBrushFromTexture(Texture);
        }
        else if(index==5)
        {
            ButtonSlot_Widget->Id = ENameBarComponentCrafting::Button5;
            FString FullPath = FPaths::ProjectContentDir() + PathImage + "flasks.png";
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
            ButtonSlot_Widget->ButtonIcon->SetBrushFromTexture(Texture);
        }

        if (!IsDesignTime())
        {
            ButtonSlot_Widget->OnClickedButton.AddUniqueDynamic(this, &UBarComponentCrafting_WC::OnClickedButton);
        }

        UHorizontalBoxSlot* HorizontalBoxSlot = HorizontalBox->AddChildToHorizontalBox(ButtonSlot_Widget);
    }
}

void UBarComponentCrafting_WC::CalculateWidgetDimension()
{
    //Dimension.X = ButtonSize.X;
    Dimension.X = 64;
    //Dimension.Y = ButtonSize.Y * NumberOfButton;
    Dimension.Y = 64 * NumberOfButton;
}

void UBarComponentCrafting_WC::OnClickedButton(const int IdButton)
{
    OnClickedBarComponentButton.Broadcast(IdButton);
}
