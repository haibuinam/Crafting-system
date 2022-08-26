// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Item/FoodItem.h"

#include "MyProject/MyProjectCharacter.h"
#include "MyProject/LogUtilities.h"


// TODO: use Tag system so that only when "Player" tag comes into overlap does this trigger `Interact`
// TODO: change all Attributes owner (Inventory, Health, etc..) to playerState at final (instead of MyProjectCharacter)
AFoodItem::AFoodItem()
{
    // setup Tick
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    TriggerVolume = CreateDefaultSubobject<USphereComponent>("Sphere Component");
    TriggerVolume->SetupAttachment(RootComponent);
    TriggerVolume->SetSphereRadius(80.f);

    TriggerVolume->SetGenerateOverlapEvents(true);
    TriggerVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &AFoodItem::OnOverlapBegin);
}

void AFoodItem::BeginPlay()
{
    Super::BeginPlay();
}

bool AFoodItem::IsPlayerInventoryFull() const
{
    if (!MyProjectCharacter)
    {
        V_LOG("MyProjectCharacter is null");
        return true;
        // we are just going to return full so that we don't have to access the array when the playerState is null
    }
    return MyProjectCharacter->Inventory->IsFull();
}

void AFoodItem::OnInteract()
{
    Super::OnInteract();
}

// TODO: Implement Destroying Item  or something for Consumables Type of Item
void AFoodItem::OnUse()
{
    Super::OnUse();

    if (MyProjectCharacter)
    {
        MyProjectCharacter->Health += HealthToHeal;
        VSCREENMSG(FColor::Yellow, "Character has %.2f health left", MyProjectCharacter->Health);

        MyProjectCharacter->Inventory->RemoveItem(this);
    }

    // TODO: Destroying the item after use
}

// The bind-ed function must have the same number of Params as the delegate that adds it (in this case `OnActorBeginOverlap`)
void AFoodItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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
