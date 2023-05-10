// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmPlant.h"
#include "Components/StaticMeshComponent.h"
#include "PlantItemDefinition.h"

// Sets default values
AFarmPlant::AFarmPlant()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(StaticMeshComponent);
}

void AFarmPlant::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	StaticMeshComponent->SetStaticMesh(PlantItemDef ? PlantItemDef->Mesh : nullptr);

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

// Called when the game starts or when spawned
void AFarmPlant::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFarmPlant::SetPlantItemDef(UPlantItemDefinition* NewPlantItemDef)
{
	PlantItemDef = NewPlantItemDef;

	StaticMeshComponent->SetStaticMesh(PlantItemDef ? PlantItemDef->Mesh : nullptr);
}

void AFarmPlant::SetGrowthPercentage(const float NewGrowthPercentage)
{
	GrowthPercentage = FMath::Clamp(NewGrowthPercentage, 0.f, 1.f);
	OnGrowthPercentageSet(GrowthPercentage);
}

const TArray<UItemDefinition*> AFarmPlant::Harvest()
{
	TArray<UItemDefinition*> HarvestedItems;
	if (PlantItemDef)
	{
		HarvestedItems.Reserve(PlantItemDef->HarvestedItems.Num());
		for (TObjectPtr<UItemDefinition> HarvestedItem : PlantItemDef->HarvestedItems)
		{
			HarvestedItems.Add(HarvestedItem);
		}
	}

	GrowthPercentage = 0.f;
	SetPlantItemDef(nullptr);

	return HarvestedItems;
}


