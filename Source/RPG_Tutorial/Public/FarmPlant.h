// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FarmPlant.generated.h"

class UItemDefinition;
class UPlantItemDefinition;

UCLASS(Blueprintable)
class RPG_TUTORIAL_API AFarmPlant : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFarmPlant();

	/** Sets how grown the plant is on a scale of 0 to 1 */
	UFUNCTION()
	void SetPlantItemDef(UPlantItemDefinition* NewPlantItemDef);

	/** Sets how grown the plant is on a scale of 0 to 1 */
	UFUNCTION(BlueprintCallable)
	void SetGrowthPercentage(const float NewGrowthPercentage);

	/** Harvests the plant and returns the yield of what is harvested from the plant */
	UFUNCTION(BlueprintCallable)
	const TArray<UItemDefinition*> Harvest();

protected:
	/** Updates the plant's mesh with the set plant item definition */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called when the growth percentage is set. Should update the plant's cosmetics */
	UFUNCTION(BlueprintImplementableEvent)
	void OnGrowthPercentageSet(const float NewGrowthPercentage);

	/** How grown the plant is. 0 for not grown at all, 1 for fully grown */
	UPROPERTY(EditAnywhere, BlueprintSetter=SetGrowthPercentage)
	float GrowthPercentage = 0.f;

	/** Item definition of the plant */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPlantItemDefinition> PlantItemDef = nullptr;

	/** Farm plant's static mesh */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
};
