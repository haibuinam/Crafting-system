// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AItemBase;
class AFoodItem;

// Update the UI when an item is added to inventory
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryUpdated, AItemBase*, Item, int32, ItemIndex);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYPROJECT_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    TArray<AItemBase*> DefaultItems;

    UPROPERTY(EditDefaultsOnly, Instanced)
    TArray<AItemBase*> Items;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory", meta=(ClampMin=0))
    int32 Capacity = 20.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(ClampMin=0))
    int32 Size = 0;

    UPROPERTY(BlueprintAssignable, Category="Inventory")
    FOnInventoryUpdated OnInventoryUpdated;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Sets default values for this component's properties
    UInventoryComponent();

    UFUNCTION()
    bool AddItem(AItemBase* Item);

    UFUNCTION()
    bool RemoveItem(AItemBase* Item);

    UFUNCTION()
    bool IsFull() const;

    UFUNCTION()
    bool IsEmpty() const;

    UFUNCTION()
    int32 GetFirstEmptyItemIndex();
    void SwapWithOccupiedIndex(const int32& IndexFrom, const int32& IndexTo);
    void SwapWithEmptyIndex(const int32& IndexFrom, const int32& IndexTo);

    UFUNCTION()
    void SwapInventoryItems(const int32& IndexFrom, const int32& IndexTo);
};
