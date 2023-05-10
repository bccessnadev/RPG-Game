// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

class UInventory;
class UItemDefinition;
class UInventoryItem;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_TUTORIAL_API IInventoryInterface
{
	GENERATED_BODY()

public:
	/** Returns the Inventory of the interfaces owner. Is required for implementation of most interface functions */
	UFUNCTION(BlueprintCallable)
	virtual UInventory* GetInventory() const = 0;

	/** Adds a item to the inventory using the definition of that item */
	UFUNCTION(BlueprintCallable)
	virtual UInventoryItem* AddInventoryItem(const UItemDefinition* ItemDef);

	/** Returns true if the inventory contains the given item */
	UFUNCTION(BlueprintCallable)
	virtual bool Contains(const UItemDefinition* ItemDef) const;

	/** Attempts to remove an item with the given item def from the inventory
	* Returns true if item was successfully removed */
	UFUNCTION(BlueprintCallable)
	virtual bool TryRemoveItem(const UItemDefinition* ItemDef);
};
