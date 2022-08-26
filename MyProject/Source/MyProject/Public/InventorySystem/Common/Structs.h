#pragma once
#include "InventorySystem/Item/ItemBase.h"
#include "MyProject/LogUtilities.h"

class AItemBase;
#include "Containers/Map.h"

#include "Structs.generated.h"

/*----- CraftingComponent -----*/
USTRUCT(BlueprintType)
struct FQuantityOfItemBaseOnIngredientList
{
    GENERATED_BODY()
    ;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSet<AItemBase*> IngredientSet;

    int CraftAmount = INT_MAX;

};

/*----- Rarity Rating (Crafting)-----*/
USTRUCT(BlueprintType)
struct FRarityRating
{
    GENERATED_BODY()
        ;
    TMap<FString, int> RarityRatingMap;
    void add()
    {
        RarityRatingMap.Add("No Rarity (Default)", -1);
        RarityRatingMap.Add("Normal", 0);
        RarityRatingMap.Add("Good", 1);
        RarityRatingMap.Add("Outstanding", 2);
        RarityRatingMap.Add("Excellent", 3);
        RarityRatingMap.Add("Masterpiece", 4);
        RarityRatingMap.Add("Legendary", 5);
        RarityRatingMap.Add("Rare", 6);
        RarityRatingMap.Add("Ultra Rare", 7);
    }
};

/*----- SortMode (Crafting)-----*/
USTRUCT(BlueprintType)
struct FSortAToZ
{
    GENERATED_BODY()
        ;
    FORCEINLINE bool operator()(const TPair<AItemBase*, TSet<AItemBase*>> &ElementA, 
                                                const TPair<AItemBase*, TSet<AItemBase*>> &ElementB) const
    {
        return ElementA.Key->DataName.ToString() < ElementB.Key->DataName.ToString();
    }

};

USTRUCT(BlueprintType)
struct FPriceHighToLow
{
    GENERATED_BODY()
        ;
    FORCEINLINE bool operator()(const TPair<AItemBase*, TSet<AItemBase*>>& ElementA,
        const TPair<AItemBase*, TSet<AItemBase*>>& ElementB) const
    {
        return ElementA.Key->ItemPrice > ElementB.Key->ItemPrice;
    }
};

USTRUCT(BlueprintType)
struct FPriceLowToHigh
{
    GENERATED_BODY()
        ;
    FORCEINLINE bool operator()(const TPair<AItemBase*, TSet<AItemBase*>>& ElementA,
        const TPair<AItemBase*, TSet<AItemBase*>>& ElementB) const
    {
        return ElementA.Key->ItemPrice < ElementB.Key->ItemPrice;
    }

};

USTRUCT(BlueprintType)
struct FRarityLowToHigh
{
    GENERATED_BODY()
        ;
    FORCEINLINE bool operator()(const TPair<AItemBase*, TSet<AItemBase*>>& ElementA,
        const TPair<AItemBase*, TSet<AItemBase*>>& ElementB) const
    {
        FRarityRating RarityRating;
        RarityRating.add();
        return RarityRating.RarityRatingMap[ElementA.Key->Rarity.ToString()] < RarityRating.RarityRatingMap[ElementB.Key->Rarity.ToString()];
    }

};

USTRUCT(BlueprintType)
struct FRarityHighToLow
{
    GENERATED_BODY()
        ;
    FORCEINLINE bool operator()(const TPair<AItemBase*, TSet<AItemBase*>>& ElementA,
        const TPair<AItemBase*, TSet<AItemBase*>>& ElementB) const
    {
        FRarityRating RarityRating;
        RarityRating.add();
        return RarityRating.RarityRatingMap[ElementA.Key->Rarity.ToString()] > RarityRating.RarityRatingMap[ElementB.Key->Rarity.ToString()];
    }

};

USTRUCT(BlueprintType)
struct FLatest
{
    GENERATED_BODY()
        ;
    FORCEINLINE bool operator()(const TPair<AItemBase*, TSet<AItemBase*>>& ElementA,
        const TPair<AItemBase*, TSet<AItemBase*>>& ElementB) const
    {
        return ElementA.Key->Date > ElementB.Key->Date;
    }

};

/*----- NameButton (Crafting)-----*/
UENUM(BlueprintType)
enum ENameSortedModeButton
{
    AToZ,
    PriceHighToLow,
    PriceLowToHigh,
    RarityHighToLow,
    RarityLowToHigh,
    Latest
};

UENUM(BlueprintType)
enum ENameOptionOfFindButton
{
    CanCraft,
    All,
    Like,
};

UENUM(BlueprintType)
enum ENameOptionOfVerticalListButton
{
    FindButton,
    ModeButton,
    SortedMode,
};

UENUM(BlueprintType)
enum ENameOfTotalAmountButton
{
    CoinButton,
    TextCoinButton,
    GoldButton,
    TextGoldButton,
    ForgingButton
};

UENUM(BlueprintType)
enum ENameOfEditQuantityButton
{
    RefreshQuantity,
    MinusButton,
    PlusButton,
    MaxButton,
};

UENUM(BlueprintType)
enum ENameBarComponentCrafting
{
    Button0,
    Button1,
    Button2,
    CraftingUI,
    EnhanceUI,
    Button5
};

/*----- Inventory -----*/
USTRUCT(BlueprintType)
struct FPadding
{
    GENERATED_BODY()
    ;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Top = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Right = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Bottom = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Left = 0;
};

USTRUCT(BlueprintType)
struct FOverrideExtraSpace
{
    GENERATED_BODY()
    ;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Horizontal = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Vertical = 0;
};


// INDEX UTILITIES
USTRUCT(BlueprintType)
struct FIndex2D
{
    GENERATED_BODY()
    ;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Row;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Col;

    int32 ConvertTo1DIndex(const int& MaxCol) const;

    bool Uninitialized() const;
};

inline int32 FIndex2D::ConvertTo1DIndex(const int& MaxCol) const
{
    return {Row * MaxCol + Col};
}

inline bool FIndex2D::Uninitialized() const
{
    return Row == -1;
}

inline FIndex2D ConvertIntTo2DIndex(const int& CurrentInt, const int& MaxCol)
{
    return {CurrentInt / MaxCol, CurrentInt % MaxCol};
}


