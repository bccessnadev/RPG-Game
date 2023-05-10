// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "UObject/NoExportTypes.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RPG_TUTORIAL_API UInventory : public UObject
{
	GENERATED_BODY()

public:
	/** Creates a item from the given item definition and adds it to the inventory */
	UFUNCTION(BlueprintCallable)
	virtual UInventoryItem* AddInventoryItem(const UItemDefinition* ItemDef);

	/** Removes the given item from the inventory */
	UFUNCTION(BlueprintCallable)
	virtual void RemoveInventoryItem(UInventoryItem* ItemToRemove);

	/** Returns the array of items inventory */
	const TArray<TObjectPtr<UInventoryItem>>& GetInventoryItems() const;

	/** Returns a item from the inventory with the given item definition */
	UFUNCTION(BlueprintCallable)
	UInventoryItem* GetItemFromDefinition(const UItemDefinition* ItemDef) const;

protected:
	/** Array of inventory items */
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UInventoryItem>> Inventory;

};
