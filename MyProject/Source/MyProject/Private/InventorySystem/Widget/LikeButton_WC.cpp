// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/LikeButton_WC.h"

#include "ImageUtils.h"
//#include "LikeButton_WC.h"

void ULikeButton_WC::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    Button->OnClicked.AddUniqueDynamic(this, &ULikeButton_WC::ClickedItem);

    //GetIcon();
}

void ULikeButton_WC::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    GetIcon();

    SizeBox->HeightOverride;
    SizeBox->WidthOverride;

    // update visibility of this widget
    this->SetVisibility(ESlateVisibility::Visible);

    // set the display to Desire On Screen
    //this->DesignSizeMode = EDesignPreviewSizeMode::DesiredOnScreen;
}

void ULikeButton_WC::ClickedItem()
{
    OnClickedLikeButton.Broadcast(Item);
}

void ULikeButton_WC::GetIcon()
{
    const FString PathImage = "ImageCrafting/";
    FString FullPath = FPaths::ProjectContentDir() + PathImage + "heart.png";
    UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
    Image->SetBrushFromTexture(Texture);
}

