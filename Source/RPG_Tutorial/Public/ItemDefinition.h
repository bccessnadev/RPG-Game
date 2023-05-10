// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDefinition.generated.h"

class UStaticMesh;
class UTexture2D;

/**
 * 
 */
UCLASS(Blueprintable)
class RPG_TUTORIAL_API UItemDefinition : public UDataAsset
{
	GENERATED_BODY(Blueprintable)
	
public:
	/** Name of the item */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName;

	/** Item's UI representation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon;

	/** Item's 3D representation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> Mesh;

	/** Can this item be equipped */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeEquipped = false;

	/** Name of the socket on the character that the item should be attached to when equipped */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString EquipSocket = TEXT("hand_r_equip");
};
