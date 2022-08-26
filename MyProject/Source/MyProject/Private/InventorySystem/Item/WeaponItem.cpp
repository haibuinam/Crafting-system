// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Item/WeaponItem.h"
#include "MyProject/MyProjectCharacter.h"
#include "MyProject/LogUtilities.h"

AWeaponItem::AWeaponItem()
{
    // setup Tick
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    TriggerVolume = CreateDefaultSubobject<UCapsuleComponent>("Capsule Component");
    TriggerVolume->SetupAttachment(RootComponent);
    TriggerVolume->InitCapsuleSize(50.f, 85.f);

    TriggerVolume->SetGenerateOverlapEvents(true);
    TriggerVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &AWeaponItem::OnOverlapBegin);
}

void AWeaponItem::BeginPlay()
{
    Super::BeginPlay();
}

bool AWeaponItem::IsPlayerInventoryFull() const
{
    if (!MyProjectCharacter)
    {
        V_LOG("MyProjectCharacter is null");
        return true;
        // we are just going to return full so that we don't have to access the array when the playerState is null
    }
    return MyProjectCharacter->Inventory->IsFull();
}

void AWeaponItem::OnInteract()
{
    Super::OnInteract();
}

// TODO: Implement Destroying Item  or something for Consumables Type of Item
void AWeaponItem::OnUse()
{
    Super::OnUse();

    if (MyProjectCharacter)
    {
        MyProjectCharacter->Damage += Damage;
        VSCREENMSG(FColor::Yellow, "Character has %.2f Damage left", MyProjectCharacter->Damage);
        MyProjectCharacter->Inventory->RemoveItem(this);
    }

    // TODO: Destroying the item after use
}

// The bind-ed function must have the same number of Params as the delegate that adds it (in this case `OnActorBeginOverlap`)
void AWeaponItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    VSCREENMSG(FColor::Red, "Character Overlapped");

    // check if the OtherActor is our Character
    if (Cast<AMyProjectCharacter>(OtherActor) != nullptr)
    {
        if (!IsPlayerInventoryFull())
        {
            // add the item to inventory Component
            MyProjectCharacter->Inventory->AddItem(this);

            // set the item un-intractable in game
            // TODO: consider Destroy the whole actor vs this approach
            this->SetActorHiddenInGame(true);
            this->SetActorEnableCollision(false);

            //x Implemented: add this item to the UI Widget has been done in InventoryComponent.Broadcast()
        }
        else
        {
            VSCREENMSG(FColor::Red, "The Inventory is full!");
        }
    }
}
