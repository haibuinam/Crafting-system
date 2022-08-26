// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting System/CraftingComponent.h"
#include "Crafting System/CraftingComponent.h"
#include "Crafting System/CraftingComponent.h"

#include "ImageUtils.h"
#include "HAL/PlatformFileManager.h"
#include "InventorySystem/Item/WeaponItem.h"
#include "InventorySystem/Item/FoodItem.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "InventorySystem/Common/Structs.h"
#include "MyProject/LogUtilities.h"
//#include "CraftingComponent.h"

// TODO P1: Add UI Invoke of craft event
// TODO P1: Add UI Update after craft

// Sets default values for this component's properties
UCraftingComponent::UCraftingComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UCraftingComponent::BeginPlay()
{
    Super::BeginPlay();

    MyProjectCharacter = Cast<AMyProjectCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

    InventoryComponent = MyProjectCharacter->Inventory;

    CraftLists.Empty();
    LoadFromJson();

    IngredientListsData = GetIngredientLists();
}

TArray<TPair<AItemBase*, TSet<AItemBase*>>>UCraftingComponent::GetIngredientLists() const
{
    TArray<TPair<AItemBase*, TSet<AItemBase*>>> IngredientLists;

    const FString FileItemJson = "IngredientLists.txt";
    FString FullItemPath = FPaths::ProjectContentDir() + FileItemJson;

    //should split this into a function?
    FString fileContent = "";
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    if (PlatformFile.FileExists(*FullItemPath))
    {
        V_LOG("File found");
        FFileHelper::LoadFileToString(fileContent, *FullItemPath);
    }
    else
    {
        V_LOG("File not found");
    }

    // get line by line in file
    TArray<FString> LinesData;

    fileContent.ParseIntoArray(LinesData, TEXT("\n"));
    for (auto &ALineData : LinesData)
    {

        TArray<FString> WordsArray;
        const TCHAR* Delims[] = { TEXT("\\"), TEXT("%")};

        ALineData.TrimStartAndEndInline();
            //V_LOG("ALine Data AfterTrim=%s=", *ALineData);
        //ALineData.TrimEndInline();
        ALineData.ParseIntoArray(WordsArray, Delims, 2);

        TSet<AItemBase*> Ingredient;
        TPair<AItemBase*, TSet<AItemBase*>> PairIngredient;

        FString TempString;
        //WordsArray.Shrink()
        while (!WordsArray.IsEmpty())
        {
            //Create Items Element
            auto a = WordsArray.Pop();
            //a.TrimEndInline();

            if(AllItemsFromJson.Find(FName(*a)))
            {
                auto ItemElement = *AllItemsFromJson.Find(FName(*a));

                //because find function will return a const& pointer. 
                auto CopyItemElement = DuplicateObject(ItemElement, nullptr);

                CopyItemElement->Amount = FCString::Atoi(*WordsArray.Pop());
                //CopyItemElement->Thumbnail = MyTexture;

                Ingredient.Add(CopyItemElement);
            }
            else
            {
                break;
            }

            if(WordsArray.Num()==1)
            {
                TempString = WordsArray.Pop();
            }
        }

        if (AllItemsFromJson.Find(FName(*TempString)))
        {
            auto ItemCanCraft = *AllItemsFromJson.Find(FName(*TempString));

            auto CopyItemCanCraft = DuplicateObject(ItemCanCraft, nullptr);

            //CopyItemCanCraft->Thumbnail = MyTexture;
            CopyItemCanCraft->Amount = 1;

            PairIngredient.Value.Append(Ingredient);
            PairIngredient.Key = CopyItemCanCraft;

            //Add TPair PairIngredient to IngredientLists
            IngredientLists.Add(PairIngredient);
        }
    }
    return IngredientLists;
}

TMap<AItemBase*, FQuantityOfItemBaseOnIngredientList> UCraftingComponent::Craft(const TArray<AItemBase*>& Inventory,
                                                                         const TArray<TPair<
                                                                             AItemBase*, TSet<AItemBase*>>>&
                                                                         IngredientLists) const
{
    // construct hash map
    TMap<FName, int> ItemToCounts;
    for (const auto& Item : Inventory)
    {
        //get only inventorySlot not nullptr
        if (Item != nullptr)
        {
            ItemToCounts.Add(Item->DataName, Item->Amount);
        }
    }

    // calc craft list
    return CalculateCraftList(ItemToCounts, IngredientLists);
}

TMap<AItemBase*, FQuantityOfItemBaseOnIngredientList> UCraftingComponent::CalculateCraftList(
    const TMap<FName, int>& ItemToCounts, const TArray<TPair<AItemBase*, TSet<AItemBase*>>>& IngredientLists) const
{
    TMap<AItemBase*, FQuantityOfItemBaseOnIngredientList> CraftList;
    int i = -1;

master_loop:
    i++;
    while (i < IngredientLists.Num())
    {
        int CraftAmount = INT_MAX;
        const auto IngredientList = IngredientLists[i];

        //TODO: think about use Append function <haibn>
        //? The Append function will move all elements from one map into the IngredientSet
        TSet<AItemBase*>IngredientSet = IngredientList.Value;
         AItemBase* MasterItem = IngredientList.Key;

        // check if we can craft the item, and the amount of item craft-able
        for (const auto& Ingredient : IngredientSet)
        {
            const int RequiredAmount = Ingredient->Amount;
            if (!ItemToCounts.Contains(Ingredient->DataName) || ItemToCounts[Ingredient->DataName] < RequiredAmount)
                goto master_loop;
            CraftAmount = FMath::Min(CraftAmount, FMath::Floor(ItemToCounts[Ingredient->DataName] / RequiredAmount));

            //UE_LOG(LogTemp, Warning, TEXT("CraftAmount1 : %d"), CraftAmount);
        }

        // push the item into Craft list
        // TODO P2: Optimize CraftList (instead of using AItemBase* as key, can we use `Name` or something?
        //CraftList[MasterItem] = TPairInitializer(CraftAmount, IngredientSet);
        //UE_LOG(LogTemp, Warning, TEXT("CraftAmount2 : %d"), CraftAmount);

        FQuantityOfItemBaseOnIngredientList QuantityOfItemBaseOnIngredientList;
        QuantityOfItemBaseOnIngredientList.CraftAmount = CraftAmount;
        QuantityOfItemBaseOnIngredientList.IngredientSet = IngredientSet;

        CraftList.Add(MasterItem, QuantityOfItemBaseOnIngredientList);

        //IngredientSet.Empty();
        i++;
    }
    return CraftList;
}

void UCraftingComponent::UpdateCraftLists()
{
    if(MyProjectCharacter->Inventory->IsEmpty())
    {
        V_LOG("CraftingComponent.cpp: Inventory is Empty!");
        return;
    }

    CraftLists = Craft (MyProjectCharacter->Inventory->Items, IngredientListsData);

    if(CraftLists.IsEmpty())
    {
        V_LOG("CraftingComponent.cpp: Craftlist is Empty!");
        return;
    }
}

void UCraftingComponent::CraftingManager(AItemBase* ItemBase, const int NumberItemPlayerWantToMake)
{
  //ToDo: Fix it <haibn> 

    if(!Validate(ItemBase, NumberItemPlayerWantToMake))
    {
        V_LOG("Validate is false");
        return;
    }

    //ham nay chac gi da dung?
    const auto Recipes = CraftLists.Find(ItemBase)->IngredientSet;

    TArray<AItemBase*> ElementList;
    // should change find function
    for(const auto &i : CraftLists)
    {
        if(ItemBase->DataName == i.Key->DataName)
        {
            ElementList = i.Value.IngredientSet.Array();
        }
    }

   /* if(Recipes.IsEmpty())
    {
        V_LOG("Recipes is empty");
    }*/
    if(ElementList.IsEmpty())
    {
        V_LOG("ElementList is null");
    }

    bool RemoveResult = true;
    TArray<AItemBase*> DeletedItems;
    auto TempInventory = MyProjectCharacter->Inventory->Items;
    //V_LOG("num() = %d, max = %d, GetSlack = %d", TempInventory.Num(), TempInventory.Max(), TempInventory.GetSlack());

    while (!ElementList.IsEmpty())
    {
        const auto ElementInArray = ElementList.Pop();

        //Find Item in Inventory && use TempInventory because avoid change during ranged-for iteration!
        for (const auto& ItemInventory : TempInventory)
        {
            if (ItemInventory)
            {
                if (ItemInventory->DataName == ElementInArray->DataName)
                {
                    for (int i = 0; i < NumberItemPlayerWantToMake * ElementInArray->Amount; i++)
                    {
                        RemoveResult = InventoryComponent->RemoveItem(ItemInventory);

                        if (!RemoveResult)
                        {
                            break;
                        }
                        DeletedItems.Add(ItemInventory);
                        //RemoveResult = false;
                    }
                }
            }
            if(!RemoveResult)
            {
                break;
            }
        }
    }

    if(RemoveResult)
    {
        for (int Amount = 0; Amount < NumberItemPlayerWantToMake; Amount++)
        {
            InventoryComponent->AddItem(ItemBase);
        }
    }
    else
    {
        while (!DeletedItems.IsEmpty())
        {
            MyProjectCharacter->Inventory->AddItem(DeletedItems.Pop());
        }
    }

    UpdateCraftLists();
}

bool UCraftingComponent::Validate(const AItemBase* ItemBase, const int NumberItemPlayerWantToMake)
{
    if (!ItemBase)
    {
        V_LOG("ItemBase is null");
        return false;
    }

    if (!CraftLists.Contains(ItemBase))
    {
        V_LOG("Item cannot craft !!!");
        return false;
    }

    if (CraftLists.Find(ItemBase)->CraftAmount < NumberItemPlayerWantToMake)
    {
        V_LOG("the Number Item player want to make > number can carft");
        return false;
    }

    if (InventoryComponent->IsFull())
    {
        V_LOG("Inventory is full");
        return false;
    }

    if(NumberItemPlayerWantToMake == 0)
    {
        V_LOG("NumberItemPlayerWantToMake = 0 !!!");
        return false;
    }

    return true;
}

//Get Items Information from Json
////////////////////////////////////////////////////////////////////////////

void UCraftingComponent::LoadFromJson()
{
    const FString FileItemJson = "ItemInformation.json";

    FString FullItemPath = FPaths::ProjectContentDir() + FileItemJson;

    FString JsonItemStr;

    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FullItemPath))
    {
        V_LOG("File Food Json was not found");
        return;
    }

    //V_LOG("Food Json found");
    FFileHelper::LoadFileToString(JsonItemStr, *FullItemPath);

    TSharedRef<TJsonReader<TCHAR>> JsonFoodReader = TJsonReaderFactory<TCHAR>::Create(JsonItemStr);
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

    const FString FileThumbnail = "Thumbnail/";
    if (FJsonSerializer::Deserialize(JsonFoodReader, JsonObject) && JsonObject.IsValid())
    {
        //add AFoodItem
        TArray<TSharedPtr<FJsonValue>> JsonFoodValuesArray = JsonObject->GetArrayField(TEXT("Food"));

        for (int32 i = 0; i < JsonFoodValuesArray.Num(); i++)
        {
            TSharedPtr<FJsonValue> JsonValue = JsonFoodValuesArray[i];
            TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

            AFoodItem* FoodItem = NewObject<AFoodItem>();
            FoodItem->DataName = FName(*JsonValueObject->GetStringField("DataName"));
            FoodItem->ItemID = JsonValueObject->GetIntegerField("ItemID");
            FoodItem->HealthToHeal = JsonValueObject->GetIntegerField("HealthToHeal");
            FoodItem->ItemPrice = JsonValueObject->GetIntegerField("ItemPrice");
            
            FDateTime TempDate{};
            auto check = FDateTime::Parse(JsonValueObject->GetStringField("Date"), TempDate);
            if(check)
            {
                FoodItem->Date = TempDate;
            }

            FoodItem->Rarity = FText::FromString(JsonValueObject->GetStringField("Rarity"));
            FoodItem->Type = FText::FromString(JsonValueObject->GetStringField("Type"));
            FoodItem->Description = FText::FromString(JsonValueObject->GetStringField("Description"));
            FoodItem->Uses = FText::FromString(JsonValueObject->GetStringField("Uses"));
            FoodItem->UseActionText = FText::FromString(JsonValueObject->GetStringField("UseActionText"));

            FString NameThumbnail = JsonValueObject->GetStringField("Thumbnail");
            FString FullThumbnailPath = FPaths::ProjectContentDir() + FileThumbnail + NameThumbnail;
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullThumbnailPath);
            FoodItem->Thumbnail = Texture;

            AllItemsFromJson.Add(FoodItem->DataName, FoodItem);
        }


        //add AWeaponItem
        TArray<TSharedPtr<FJsonValue>> JsonWeaponValuesArray = JsonObject->GetArrayField(TEXT("Weapon"));

        for (int32 i = 0; i < JsonWeaponValuesArray.Num(); i++)
        {
            TSharedPtr<FJsonValue> JsonValue = JsonWeaponValuesArray[i];
            TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

            AWeaponItem* WeaponItem = NewObject<AWeaponItem>();
            WeaponItem->DataName = FName(*JsonValueObject->GetStringField("DataName"));
            WeaponItem->ItemID = JsonValueObject->GetIntegerField("ItemID");
            WeaponItem->Damage = JsonValueObject->GetIntegerField("Damage");
            WeaponItem->ItemPrice = JsonValueObject->GetIntegerField("ItemPrice");

            FDateTime TempDate{};
            auto check = FDateTime::Parse(JsonValueObject->GetStringField("Date"), TempDate);
            if (check)
            {
                WeaponItem->Date = TempDate;
            }

            WeaponItem->Rarity = FText::FromString(JsonValueObject->GetStringField("Rarity"));
            WeaponItem->Type = FText::FromString(JsonValueObject->GetStringField("Type"));
            WeaponItem->Description = FText::FromString(JsonValueObject->GetStringField("Description"));
            WeaponItem->Uses = FText::FromString(JsonValueObject->GetStringField("Uses"));
            WeaponItem->UseActionText = FText::FromString(JsonValueObject->GetStringField("UseActionText"));

            FString NameThumbnail = JsonValueObject->GetStringField("Thumbnail");
            FString FullThumbnailPath = FPaths::ProjectContentDir() + FileThumbnail + NameThumbnail;
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullThumbnailPath);
            WeaponItem->Thumbnail = Texture;

            AllItemsFromJson.Add(WeaponItem->DataName, WeaponItem);
        }

        //add ItemBase
        TArray<TSharedPtr<FJsonValue>> JsonItemBaseValuesArray = JsonObject->GetArrayField(TEXT("ItemBase"));

        for (int32 i = 0; i < JsonItemBaseValuesArray.Num(); i++)
        {
            TSharedPtr<FJsonValue> JsonValue = JsonItemBaseValuesArray[i];
            TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

            //TODO: change Food Item -> Itembase or other class <haibn>
            AFoodItem* ItemBase = NewObject<AFoodItem>();
            ItemBase->DataName = FName(*JsonValueObject->GetStringField("DataName"));
            ItemBase->ItemID = JsonValueObject->GetIntegerField("ItemID");
            //ItemBase->Damage = JsonValueObject->GetIntegerField("Damage");
            ItemBase->ItemPrice = JsonValueObject->GetIntegerField("ItemPrice");

            FDateTime TempDate{};
            auto check = FDateTime::Parse(JsonValueObject->GetStringField("Date"), TempDate);
            if (check)
            {
                ItemBase->Date = TempDate;
            }

            ItemBase->Rarity = FText::FromString(JsonValueObject->GetStringField("Rarity"));
            ItemBase->Type = FText::FromString(JsonValueObject->GetStringField("Type"));

            FString NameThumbnail = JsonValueObject->GetStringField("Thumbnail");
            FString FullThumbnailPath = FPaths::ProjectContentDir() + FileThumbnail + NameThumbnail;
            UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(FullThumbnailPath);
            ItemBase->Thumbnail = Texture;

            AllItemsFromJson.Add(ItemBase->DataName, ItemBase);
        }
    }
   
}
