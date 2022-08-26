// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/Option_OfVerticalList_WC.h"

#include "ImageUtils.h"
#include "Components/HorizontalBoxSlot.h"
#include "Crafting System/CraftingComponent.h"
#include "GameFramework/HUD.h"
#include "MyProject/LogUtilities.h"
#include "MyProject/MyProjectCharacter.h"
//#include "Option_OfVerticalList_WC.h"

void UOption_OfVerticalList_WC::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    MyProjectCharacter = Cast<AMyProjectCharacter>(GetOwningPlayerPawn());

    CraftingComponent = MyProjectCharacter->Crafting;

    InitOption_OfVerticalList_Widget();

    //Border->SetVisibility(ESlateVisibility::Hidden);
    bIsFocusable = true;
}

void UOption_OfVerticalList_WC::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    EditableTextSize->WidthOverride;
    EditableTextSize->HeightOverride;

    InitOption_OfVerticalList_Widget();
    CalculateWidgetDimension();

     //set the proper width for SizeBox
    SizeBox->SetWidthOverride(Dimension.Y);
    SizeBox->SetHeightOverride(Dimension.X);

    // update visibility of this widget
    this->SetVisibility(ESlateVisibility::Visible);

    // set the display to Desire On Screen
    this->DesignSizeMode = EDesignPreviewSizeMode::DesiredOnScreen;
}

void UOption_OfVerticalList_WC::InitOption_OfVerticalList_Widget()
{
    if(!OptionOfVerticalListSlotClass)
    {
        return;
    }

    HorizontalBoxButtons->ClearChildren();

    EditableTextFindItemByName->OnTextChanged.AddUniqueDynamic(this, &UOption_OfVerticalList_WC::OnTextChangedWhenPlayerClicked);

    EditableTextFindItemByName->OnTextCommitted.AddUniqueDynamic(this, &UOption_OfVerticalList_WC::OnTextCommitWhenPlayerClicked);

    for(int i =0 ; i<NumberOfButton; i++)
    {
        const auto ButtonSlot_Widget = CreateWidget<UCraftingButton_WC>(this, OptionOfVerticalListSlotClass);
        ButtonSize = ButtonSlot_Widget->Dimension;

        ButtonSlot_Widget->Id = i;

        const FString PathImage = "ImageCrafting/";
        if(i ==0)
        {
            //ButtonSlot_Widget->ButtonName = FName(TEXT("FindButton"));
            ButtonSlot_Widget->Id = ENameOptionOfVerticalListButton::FindButton;
            FString FullPath = FPaths::ProjectContentDir() + PathImage + "magnifying glass.png";
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
            ButtonSlot_Widget->ButtonIcon->SetBrushFromTexture(Texture);
        }
        else if (i ==1)
        {
            //ButtonSlot_Widget->ButtonName = FName(TEXT("ModeButton"));
            ButtonSlot_Widget->Id = ENameOptionOfVerticalListButton::ModeButton;
            FString FullPath = FPaths::ProjectContentDir() + PathImage + "funnel.png";
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
            ButtonSlot_Widget->ButtonIcon->SetBrushFromTexture(Texture);
        }
        else if (i ==2)
        {
            //ButtonSlot_Widget->ButtonName = FName(TEXT("SortedMode"));
            ButtonSlot_Widget->Id = ENameOptionOfVerticalListButton::SortedMode;
            FString FullPath = FPaths::ProjectContentDir() + PathImage + "Arrow.png";
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
            ButtonSlot_Widget->ButtonIcon->SetBrushFromTexture(Texture);
        }

        // Bind OnClicked functions
        if (!IsDesignTime())
        {
            ButtonSlot_Widget->OnClickedButton.AddUniqueDynamic(this, &UOption_OfVerticalList_WC::OnClickedButton);
        }

        //add to UI
        UHorizontalBoxSlot* HorizontalBoxSlot = HorizontalBoxButtons->AddChildToHorizontalBox(ButtonSlot_Widget);
        OptionVerticalListSlot_Widgets.Add(HorizontalBoxSlot);

        HorizontalBoxSlot->SetHorizontalAlignment(HAlign_Center);
    }

}

void UOption_OfVerticalList_WC::CalculateWidgetDimension()
{
    Dimension.X = 64; /*+ FindButtonSize->HeightOverride + ModeButtonSize->HeightOverride
        + SortedModeSize->HeightOverride;*/
    Dimension.Y = 64 * NumberOfButton + EditableTextSize->WidthOverride; //EditableTextFindItemByName.Y = 196;
}

//link:Option_OfVerticalList_WC.cpp:"InitOption_OfVerticalList_Widget"
void UOption_OfVerticalList_WC::OnClickedButton(const int IdOptionVerticalListButton)
{
    if(IdOptionVerticalListButton== ENameOptionOfVerticalListButton::FindButton)
    {
        auto TextInput = EditableTextFindItemByName->GetText().ToString();
        if(TextInput.IsEmpty())
        {
            V_LOG("Text input is empty");
            return;
        }

        CraftingComponent->ResultItemsListAfterSearching.Empty();
        for(const auto Item : CraftingComponent->IngredientListsData)
        {
            auto NameItem = Item.Key->DataName.ToString();
            //set number ==2
            if(ReportAfterSearching(NameItem, TextInput, 2))
            {
                CraftingComponent->ResultItemsListAfterSearching.Add(Item.Key);
            }
        }
    }

    OnClickedButtonOfVerticalList.Broadcast(IdOptionVerticalListButton);
}

bool UOption_OfVerticalList_WC:: ReportAfterSearching (FString NameItem, FString TextInput, int NumberOfCharactersAllowedToBeWrong)
{
    //don't need to check NameItem && TextInput is empty.
    //set length TexInput = 31
    if(TextInput.Len()>31)
    {
        V_LOG("Text Input lenght > 31");
        return false;
    }

    if (TextInput.Equals(NameItem)) return true;

    if (TextInput.Contains(NameItem)) return true;

    auto NameItemToLower = NameItem.ToLower();
    auto TextInputToLower = TextInput.ToLower();

    int Index = INDEX_NONE;

    for(int i=0; i< TextInput.Len(); i++)
    {
        Index = NameItemToLower.FindChar(TextInputToLower[i],Index);
        if(TextInputToLower.Len()==1)
        {
            if(NameItemToLower[0] == TextInputToLower[0])
            {
                return true;
            }
            return false;
        }

        if (TextInputToLower.Len() == 2)
        {
            if (NameItemToLower[0] == TextInputToLower[0]&& NameItemToLower[1] == TextInputToLower[1])
            {
                return true;
            }
            return false;
        }

        if(!Index)
        {
            NumberOfCharactersAllowedToBeWrong--;
        }
        if(NumberOfCharactersAllowedToBeWrong < 0)
        {
            return false;
        }
    }
    return true;
}

//link:Option_OfVerticalList_WC.cpp:"InitOption_OfVerticalList_Widget"
void UOption_OfVerticalList_WC::OnTextChangedWhenPlayerClicked(const FText& InText)
{
    //TODO: code suggestions <haibn>
    //thu dung button goi y xem?
}

//link:Option_OfVerticalList_WC.cpp:"InitOption_OfVerticalList_Widget"
void UOption_OfVerticalList_WC::OnTextCommitWhenPlayerClicked(const FText& InText, ETextCommit::Type InCommitType)
{
    OnClickedButton(ENameOptionOfVerticalListButton::FindButton);
}


