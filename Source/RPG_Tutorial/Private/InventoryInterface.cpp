// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryInterface.h"
#include "Inventory.h"
#include "ItemDefinition.h"

UInventoryItem* IInventoryInterface::AddInventoryItem(const UItemDefinition* ItemDef)
{
	if (UInventory* Inventory = GetInventory())
	{
		return Inventory->AddInventoryItem(ItemDef);
	}

	return nullptr;
}

bool IInventoryInterface::Contains(const UItemDefinition* ItemDef) const
{
	if (const UInventory* Inventory = GetInventory())
	{
		if (Inventory->GetItemFromDefinition(ItemDef))
		{
			return true;
		}
	}
	
	return false;
}

bool IInventoryInterface::TryRemoveItem(const UItemDefinition* ItemDef)
{
	if (UInventory* Inventory = GetInventory())
	{
		if (UInventoryItem* ItemToRemove = Inventory->GetItemFromDefinition(ItemDef))
		{
			Inventory->RemoveInventoryItem(ItemToRemove);
			return true;
		}
	}
	
	return false;
}