// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/CraftingButton_WC.h"

#include "MyProject/MyProjectCharacter.h"
//#include "CraftingButton_WC.h"

void UCraftingButton_WC::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    //MyProjectCharacter = Cast<AMyProjectCharacter>(GetOwningPlayerPawn());
    Button->OnClicked.AddUniqueDynamic(this, &UCraftingButton_WC::SendIdCraftingButton);
}

void UCraftingButton_WC::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    Dimension.X = SizeBox->WidthOverride;
    Dimension.Y = SizeBox->HeightOverride;
}

void UCraftingButton_WC::SendIdCraftingButton()
{
    OnClickedButton.Broadcast(Id);
}
