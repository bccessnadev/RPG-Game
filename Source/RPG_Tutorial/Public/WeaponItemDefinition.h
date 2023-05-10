// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "WeaponItemDefinition.generated.h"

/**
 * 
 */
UCLASS()
class RPG_TUTORIAL_API UWeaponItemDefinition : public UItemDefinition
{
	GENERATED_BODY()
	
public:
	/** How much damage is applied when hitting something */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;
};
