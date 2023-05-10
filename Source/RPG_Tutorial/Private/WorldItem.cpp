// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldItem.h"
#include "InventoryInterface.h"
#include "ItemDefinition.h"
#include "Components/StaticMeshComponent.h"

AWorldItem::AWorldItem()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(StaticMeshComponent);

	InteractText = TEXT("Pickup");
}

void AWorldItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (ItemDefinition)
	{
		StaticMeshComponent->SetStaticMesh(ItemDefinition->Mesh);
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

UItemDefinition* AWorldItem::GetItemDefinition() const
{
	return ItemDefinition;
}

void AWorldItem::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AWorldItem::Interact(AActor* InteractingActor)
{
	if (IInventoryInterface* InteractingInventory = Cast<IInventoryInterface>(InteractingActor))
	{
		InteractingInventory->AddInventoryItem(GetItemDefinition());
		Destroy();
		return true;
	}

	return false;
}

