// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "InventoryItem.h"
#include "ItemDefinition.h"

UInventoryItem* UInventory::AddInventoryItem(const UItemDefinition* ItemDef)
{
	if (UInventoryItem* NewItem = NewObject<UInventoryItem>(this))
	{
		NewItem->ConstructFromItemDefinition(ItemDef);
		Inventory.Add(NewItem);
		return NewItem;
	}

	return nullptr;
}

void UInventory::RemoveInventoryItem(UInventoryItem* ItemToRemove)
{
	Inventory.RemoveSwap(ItemToRemove);
}

const TArray<TObjectPtr<UInventoryItem>>& UInventory::GetInventoryItems() const
{
	return Inventory;
}

UInventoryItem* UInventory::GetItemFromDefinition(const UItemDefinition* ItemDef) const
{
	if (const TObjectPtr<UInventoryItem>* FoundItem = Inventory.FindByPredicate([ItemDef](TObjectPtr<UInventoryItem> Item)
		{ return Item->ItemDefinition == ItemDef; }))
	{
		return *FoundItem;
	}

	return nullptr;
}