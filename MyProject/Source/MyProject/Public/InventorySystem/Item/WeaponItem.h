// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Components/SphereComponent.h"
#include "InventorySystem/Item/ItemBase.h"
#include "Components/CapsuleComponent.h"

#include "WeaponItem.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API AWeaponItem : public AItemBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, Category = "Inventory|Item")
    float Damage = -INT_MAX; // this is for debug purpose

    // setting the collision box to have spherical shape
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Item")
    UCapsuleComponent* TriggerVolume = nullptr;

protected:
    virtual void OnInteract() override; // the interact here is pickup?

    virtual void OnUse() override; // the use here is using the item

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                        UPrimitiveComponent* OtherComp,
                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
    AWeaponItem();

    virtual void BeginPlay() override;

    bool IsPlayerInventoryFull() const;
};
