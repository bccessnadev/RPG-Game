// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "PlantItemDefinition.generated.h"

class UItemDefinition;

/**
 * 
 */
UCLASS()
class RPG_TUTORIAL_API UPlantItemDefinition : public UItemDefinition
{
	GENERATED_BODY()

public:
	/** How long it takes for the plant to finish growing */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GrowthTime = 60.0f;

	/** Items gained when harvesting the plant */
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UItemDefinition>> HarvestedItems;
};
