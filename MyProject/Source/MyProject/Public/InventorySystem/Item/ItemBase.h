// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventorySystem/Common/Constants.h"
#include "InventorySystem/Common/InteractableInterface.h"
//#include "MyProject/MyProjectCharacter.h"

#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MYPROJECT_API AItemBase : public AActor, public IInteractableInterface
{
    GENERATED_BODY()


public:
    /*------ CODE DATA ------*/
    UPROPERTY()
    UWorld* World = nullptr;

    UPROPERTY()
    class AMyPlayerController* MyPlayerController = nullptr;

    UPROPERTY()
    class AMyProjectCharacter* MyProjectCharacter = nullptr;

    UPROPERTY()
    class AMyPlayerState* MyPlayerState = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Common")
    class USceneComponent* SceneRootComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Item")
    class UInventoryComponent* OwningInventory = nullptr;

    // index in the current Inventory array, for usage here:
    // link:InventoryComponent.cpp:"bool UInventoryComponent::RemoveItem(AItemBase* Item)"
    UPROPERTY()
    int32 Index1D = INDEX_NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Item|Metadata")
    FName DataName = ITEM_NAME_DEFAULT;

    /*------ BLUEPRINT DATA ------*/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Item", DisplayName="Static Mesh")
    UStaticMeshComponent* Mesh = nullptr;

    /*------ SQL ------*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Item")
    int32 ItemID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Item|Metadata")
    FText UseActionText = ITEM_USE_ACTION_TEXT_DEFAULT;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Item|Metadata")
    FText Uses = ITEM_USES_TEXT_DEFAULT;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Item|Metadata")
    UTexture2D* Thumbnail = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Item|Metadata")
    FText DisplayName = ITEM_DISPLAY_NAME_DEFAULT;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Item|Metadata")
    FText Description = ITEM_DESCRIPTION_DEFAULT;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Item|Metadata")
    float Weight = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Item|Metadata")
    int ItemPrice = INT_MAX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Item|Metadata")
    FDateTime Date;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Item|Metadata")
    FText Rarity = ITEM_RARITY_DEFAULT;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Item|Metadata", DisplayName="Is Stackable ?")
    bool bIsStackable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item|Metadata")
    int32 Amount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item|Metadata")
    FText Style = ITEM_EQUIP_STYLE_DEFAULT;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item|Metadata")
    FText Type = ITEM_EQUIP_TYPE_DEFAULT;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item|Metadata")
    FText Essence = ITEM_ESSENCE_DEFAULT;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item|Metadata")
    FText EquipSocket = ITEM_EQUIP_SOCKET_DEFAULT;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item|Metadata")
    bool bTradable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item|Metadata")
    bool bBankable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item|Metadata")
    bool bEquipable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item|Metadata")
    bool bDisassemable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item|Metadata")
    bool bCraftable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item|Metadata")
    bool bDestroyable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item|Metadata")
    bool bDropOnDeath = false;

public:
    AItemBase();

    virtual void BeginPlay() override;

    virtual void OnInteract() override;

    virtual void OnUse();

    //change from this->ItemID == that -> ItemID to this->DataName == that->DataName 
    FORCEINLINE bool operator==(const AItemBase* that) const
    {
        return this->DataName == that->DataName;
    }

    FORCEINLINE bool operator!=(const AItemBase* that) const
    {
        return !operator==(that);
    }
    FORCEINLINE bool operator<(const AItemBase* that) const
    {
        return this->DataName.Compare(that->DataName) < 0;
    }
};
