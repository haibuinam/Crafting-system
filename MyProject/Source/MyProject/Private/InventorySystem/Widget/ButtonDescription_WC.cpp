// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/ButtonDescription_WC.h"

#include "MyProject/LogUtilities.h"
#include "MyProject/MyProjectCharacter.h"
//#include "CraftingButton_WC.h"

void UButtonDescription_WC::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    MyProjectCharacter = Cast<AMyProjectCharacter>(GetOwningPlayerPawn());
    Button->OnClicked.AddUniqueDynamic(this, &UButtonDescription_WC::IdButtonOnClicked);
}

void UButtonDescription_WC::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    Dimension.X = SizeBox->WidthOverride;
    Dimension.Y = SizeBox->HeightOverride;
}

void UButtonDescription_WC::IdButtonOnClicked()
{
    OnClickedButtonDescription.Broadcast(Id);
}


