// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySystem/Inventory/InventoryComponent.h"
#include "MyProject/MyProjectCharacter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Containers/Map.h"
#include "InventorySystem/Common/Structs.h"

#include "CraftingComponent.generated.h"

class AItemBase;
class AxeItem;
class FoodItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYPROJECT_API UCraftingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /*----- DATA -----*/
    UPROPERTY()
    AMyProjectCharacter* MyProjectCharacter;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Inventory")
    class UInventoryComponent* InventoryComponent;

    UPROPERTY()
    bool CraftListsIsChanged = false;

    //TODO: save ListFavoritesItems in file txt or something <haibn>
    UPROPERTY()
    TArray<AItemBase*> ListFavoritesItems;

    UPROPERTY()
    TArray<AItemBase*> ResultItemsListAfterSearching;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Sets default values for this component's properties
    UCraftingComponent();

    //should use UPROPERTY?
    UPROPERTY()
    TMap<AItemBase*, FQuantityOfItemBaseOnIngredientList> CraftLists;

    TMap<AItemBase*, FQuantityOfItemBaseOnIngredientList> Craft(const TArray<AItemBase*>& Inventory,
                                                         const TArray<TPair<AItemBase*, TSet<AItemBase*>>>&
                                                         IngredientLists) const;

    TMap<AItemBase*, FQuantityOfItemBaseOnIngredientList> CalculateCraftList(const TMap<FName, int>& ItemToCounts,
                                                                      const TArray<TPair<AItemBase*, TSet<AItemBase*>>>&
                                                                      IngredientLists) const;

    TArray<TPair<AItemBase*, TSet<AItemBase*>>>GetIngredientLists() const;


    TArray<TPair<AItemBase*, TSet<AItemBase*>>> IngredientListsData;

    UFUNCTION()
    void UpdateCraftLists();

    UFUNCTION()
    void CraftingManager(AItemBase* ItemBase, const int NumberItemPlayerWantToMake);

    UFUNCTION()
    bool Validate(const AItemBase* ItemBase, const int NumberItemPlayerWantToMake);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JsonData")
    AFoodItem* ItemFromJson;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JsonData")
    TArray<AItemBase*> ItemsJson;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JsonData")
    TMap<FName, AItemBase*> AllItemsFromJson;

    UFUNCTION()
    void LoadFromJson();
};
