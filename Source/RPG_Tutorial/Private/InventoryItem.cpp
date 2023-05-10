// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"
#include "ItemDefinition.h"

void UInventoryItem::ConstructFromItemDefinition(const UItemDefinition* ItemDef)
{
	ItemDefinition = ItemDef;
}