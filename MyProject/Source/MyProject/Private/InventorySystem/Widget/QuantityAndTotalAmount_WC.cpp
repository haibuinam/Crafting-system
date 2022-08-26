// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Widget/QuantityAndTotalAmount_WC.h"

#include "ImageUtils.h"
#include "Components/HorizontalBoxSlot.h"
#include "Crafting System/CraftingComponent.h"
#include "InventorySystem/Common/Structs.h"
#include "MyProject/MyProjectCharacter.h"
#include "MyProject/LogUtilities.h"

void UQuantityAndTotalAmount_WC::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    MyProjectCharacter = Cast<AMyProjectCharacter>(GetOwningPlayerPawn());

    CraftingComponent = MyProjectCharacter->Crafting;
}

void UQuantityAndTotalAmount_WC::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    InitQuantityAndTotalAmount();
    CalculateWidgetDimension();

    // update visibility of this widget
    this->SetVisibility(ESlateVisibility::Visible);

    // set the display to Desire On Screen
    this->DesignSizeMode = EDesignPreviewSizeMode::DesiredOnScreen;
}

void UQuantityAndTotalAmount_WC::InitQuantityAndTotalAmount()
{
    if(!CraftingButtonSlotClass)
    {
        V_LOG("Button Description Slot Class is Null");
        return;
    }

    InitTotalAmount();
    InitQuantity();
}

void UQuantityAndTotalAmount_WC::InitTotalAmount()
{
    TotalAmountHorizontalBox->ClearChildren();
    for(int Number = 0; Number<TotalAmountButtonNumber; Number++)
    {
        const auto DescriptionButton_Widget = CreateWidget<UCraftingButton_WC>(this, CraftingButtonSlotClass);

        //set gold text to index 1
        if (Number == 1)
        {
            UHorizontalBoxSlot* HorizontalBoxSlot = TotalAmountHorizontalBox->AddChildToHorizontalBox(GoldText);
            HorizontalBoxSlot->SetHorizontalAlignment(HAlign_Center);
            HorizontalBoxSlot->SetVerticalAlignment(VAlign_Center);
            Number++;
        }

        //set gold text to index 3
        if (Number == 3)
        {
            UHorizontalBoxSlot* HorizontalBoxSlot = TotalAmountHorizontalBox->AddChildToHorizontalBox(RubyText);
            HorizontalBoxSlot->SetHorizontalAlignment(HAlign_Center);
            HorizontalBoxSlot->SetVerticalAlignment(VAlign_Center);
            Number++;
        }

        const FString PathImage = "ImageCrafting/";
        if(Number==0)
        {
            DescriptionButton_Widget->Id = ENameOfTotalAmountButton::CoinButton;
            FString FullPath = FPaths::ProjectContentDir() + PathImage + "gold.png";
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
            DescriptionButton_Widget->ButtonIcon->SetBrushFromTexture(Texture);
        }
        
        else if (Number == 2)
        {
            DescriptionButton_Widget->Id = ENameOfTotalAmountButton::GoldButton;
            FString FullPath = FPaths::ProjectContentDir() + PathImage + "ruby.png";
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
            DescriptionButton_Widget->ButtonIcon->SetBrushFromTexture(Texture);
        }
        
        else if(Number == 4)
        {
            DescriptionButton_Widget->Id = ENameOfTotalAmountButton::ForgingButton;
            FString FullPath = FPaths::ProjectContentDir() + PathImage +"Crafting.png";
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
            DescriptionButton_Widget->ButtonIcon->SetBrushFromTexture(Texture);
        }

        DescriptionButton_Widget->OnClickedButton.AddUniqueDynamic(this, &UQuantityAndTotalAmount_WC::ClickTotalAmountHorizontalBox);
        UHorizontalBoxSlot* HorizontalBoxSlot = TotalAmountHorizontalBox->AddChildToHorizontalBox(DescriptionButton_Widget);
    }
}

void UQuantityAndTotalAmount_WC::InitQuantity()
{
    EditQuantityHorizontalBox->ClearChildren();
    for(int Number =0; Number<QuantityButtonNumber; Number++)
    {
        const auto DescriptionButton_Widget = CreateWidget<UCraftingButton_WC>(this, CraftingButtonSlotClass);


        if (Number == 2)
        {
            //we want put EditableTextBox_OfItem center/midle Quantity Widget
            UHorizontalBoxSlot* HorizontalBoxSlot = EditQuantityHorizontalBox->AddChildToHorizontalBox(SizeEditableOfTextBox);
            Number++;
        }

        const FString PathImage = "ImageCrafting/";
        if (Number == 0)
        {
            DescriptionButton_Widget->Id = ENameOfEditQuantityButton::RefreshQuantity;
            FString FullPath = FPaths::ProjectContentDir() + PathImage + "refresh.png";
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
            DescriptionButton_Widget->ButtonIcon->SetBrushFromTexture(Texture);
        }
        else if (Number == 1)
        {
            DescriptionButton_Widget->Id = ENameOfEditQuantityButton::MinusButton;
            //DescriptionButton_Widget->Text->SetText(FText::AsCultureInvariant("-1"));
            FString FullPath = FPaths::ProjectContentDir() + PathImage + "down.png";
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
            DescriptionButton_Widget->ButtonIcon->SetBrushFromTexture(Texture);
        }
        else if (Number == 3)
        {
            DescriptionButton_Widget->Id = ENameOfEditQuantityButton::PlusButton;
            //DescriptionButton_Widget->Text->SetText(FText::AsCultureInvariant("+1"));
            FString FullPath = FPaths::ProjectContentDir() + PathImage + "up.png";
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
            DescriptionButton_Widget->ButtonIcon->SetBrushFromTexture(Texture);
        }
        else if (Number == 4)
        {
            DescriptionButton_Widget->Id = ENameOfEditQuantityButton::MaxButton;
            //DescriptionButton_Widget->Text->SetText(FText::AsCultureInvariant("Max"));
            FString FullPath = FPaths::ProjectContentDir() + PathImage + "MAXXX.png";
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullPath);
            DescriptionButton_Widget->ButtonIcon->SetBrushFromTexture(Texture);

        }
        DescriptionButton_Widget->OnClickedButton.AddUniqueDynamic(this, &UQuantityAndTotalAmount_WC::ClickEditQuantityHorizontalBox);
        UHorizontalBoxSlot* HorizontalBoxSlot = EditQuantityHorizontalBox->AddChildToHorizontalBox(DescriptionButton_Widget);
        HorizontalBoxSlot->SetHorizontalAlignment(HAlign_Center);
    }
}

void UQuantityAndTotalAmount_WC::CalculateWidgetDimension()
{
    //TODO: must fix this. <haibn>
    TotalAmountDimension.X = SizeOfTotalAmount->WidthOverride;
    TotalAmountDimension.Y = SizeOfTotalAmount->HeightOverride;

    QuantityDimension.X = SizeEditableOfTextBox->WidthOverride;
    QuantityDimension.Y = SizeEditableOfTextBox->HeightOverride;

    SizeEditQuantityHorizontalBox->WidthOverride;
    SizeEditQuantityHorizontalBox->HeightOverride;

    Dimension.X = SizeBox->WidthOverride;
    Dimension.Y = SizeBox->HeightOverride;
}

void UQuantityAndTotalAmount_WC::GetSelectedItem(AItemBase* Item)
{
    SelectedItem = Item;
    GoldText->SetText(FText::AsNumber(Item->ItemPrice));
    RubyText->SetText(FText::AsNumber(Item->ItemPrice-10));
}

int UQuantityAndTotalAmount_WC::GetNumberFromEditableText(FText Input)
{
    if(Input.IsEmpty())
    {
        return 0;
    }

    if(Input.IsNumeric())
    {
        if(FCString::Atoi(*Input.ToString()) >= 0)
        {
            return FCString::Atoi(*Input.ToString());
        }
        return -FCString::Atoi(*Input.ToString());
    }

    const FRegexPattern myPattern(TEXT("[1-9][0-9]*"));

    FRegexMatcher myMatcher(myPattern, *Input.ToString());

    int32 IndexBeginMatch = INDEX_NONE;
    int32 IndexEndMatch = INDEX_NONE;
    auto CopyInput = Input.ToString();

    FString InputNumber = "";

    //get all positive number && get only Integer
    while (myMatcher.FindNext())
    {
         IndexBeginMatch = myMatcher.GetMatchBeginning();
         IndexEndMatch = myMatcher.GetMatchEnding();

         if (IndexBeginMatch == INDEX_NONE || IndexEndMatch == INDEX_NONE)
         {
             return 0;
         }

         for (; IndexBeginMatch < IndexEndMatch; IndexBeginMatch++)
         {
             InputNumber = InputNumber + CopyInput[IndexBeginMatch];
         }
    }
    return FCString::Atoi(*InputNumber);
}

//link:QuantityAndTotalAmount_WC.cpp:"InitQuantity()"
void UQuantityAndTotalAmount_WC::ClickEditQuantityHorizontalBox(const int IdButtonEditQuantity)
{
    if(!SelectedItem)
    {
        V_LOG("Selected Item is nullptr / player hasn't selected any Item yet");
        return;
    }

    const auto PlayerInput = EditNumberOfItem->GetText();
    auto NumberInput = GetNumberFromEditableText(PlayerInput);

    if(IdButtonEditQuantity == ENameOfEditQuantityButton::RefreshQuantity)
    {
        EditNumberOfItem->SetText(FText::FromString(FString::FromInt(0)));
        return;
    }

    if (IdButtonEditQuantity == ENameOfEditQuantityButton::MinusButton)
    {
        if(NumberInput == 0)
            return;

        NumberInput--;
    }

    if (IdButtonEditQuantity == ENameOfEditQuantityButton::PlusButton)
    {
        NumberInput++;
    }

    if (IdButtonEditQuantity == ENameOfEditQuantityButton::MaxButton)
    {
        //check Selected Item can Craft or not
        if(CraftingComponent->CraftLists.Contains(SelectedItem))
        {
            NumberInput = CraftingComponent->CraftLists.Find(SelectedItem)->CraftAmount;
        }
        else
        {
            EditNumberOfItem->SetText(FText::FromString(FString::FromInt(0)));
            return;
        }
    }
    EditNumberOfItem->SetText(FText::FromString(FString::FromInt(NumberInput)));
}

//link:QuantityAndTotalAmount_WC.cpp:"InitTotalAmount"
void UQuantityAndTotalAmount_WC::ClickTotalAmountHorizontalBox(const int IdButtonTotalAmount)
{
    if (!SelectedItem)
    {
        V_LOG("Selected Item is nullptr / player hasn't selected any Item yet");
        return;
    }

    if(IdButtonTotalAmount == ENameOfTotalAmountButton::ForgingButton)
    {
        const auto PlayerInput = EditNumberOfItem->GetText();
        const auto NumberInput = GetNumberFromEditableText(PlayerInput);

        EditNumberOfItem->SetText(FText::FromString(FString::FromInt(NumberInput)));

        //if(CraftingComponent->CraftLists.Find(SelectedItem)->CraftAmount <NumberInput)
        //{
            //V_LOG("Number input > number can craft");
            //return;
        //}
        OnClickedForgingButton.Broadcast(SelectedItem, NumberInput);

    }
    //the number of Items player want to make
}
