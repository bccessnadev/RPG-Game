// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItem.generated.h"

class UItemDefinition;

/**
 * 
 */
UCLASS(Blueprintable)
class RPG_TUTORIAL_API UInventoryItem : public UObject
{
	GENERATED_BODY()

public:
	/** Instantiates the item from a ItemDefinition*/
	virtual void ConstructFromItemDefinition(const UItemDefinition* ItemDef);

	/** Definition of this item instance */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<const UItemDefinition> ItemDefinition;

};
