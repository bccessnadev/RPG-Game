// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "SeedItemDefinition.generated.h"

class UPlantItemDefinition;

/**
 * 
 */
UCLASS()
class RPG_TUTORIAL_API USeedItemDefinition : public UItemDefinition
{
	GENERATED_BODY()

public:
	/** Plant that is grown by the seeds */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPlantItemDefinition> PlantItemDef = nullptr;
};
