// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmPlot.h"
#include "Components/ChildActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FarmPlant.h"
#include "Inventory.h"
#include "InventoryItem.h"
#include "InventoryInterface.h"
#include "PlantItemDefinition.h"
#include "SeedItemDefinition.h"

// Sets default values
AFarmPlot::AFarmPlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(StaticMeshComponent);

	PlantActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("PlantActorComponent"));
	PlantActorComponent->SetupAttachment(RootComponent);
	PlantActorComponent->SetChildActorClass(AFarmPlant::StaticClass());
}

void AFarmPlot::PostRegisterAllComponents()
{
	PlantChildActor = Cast<AFarmPlant>(PlantActorComponent->GetChildActor());
}

// Called when the game starts or when spawned
void AFarmPlot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFarmPlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FarmPlotState == EFarmPlotState::Growing)
	{
		CurrentGrowTime = FMath::Min(CurrentGrowTime + DeltaTime, GrowthTime);

		if (PlantChildActor.IsValid() && GrowthTime > 0.f)
		{
			PlantChildActor->SetGrowthPercentage(CurrentGrowTime / GrowthTime);
		}

		if (CurrentGrowTime == GrowthTime)
		{
			FarmPlotState = EFarmPlotState::Grown;
		}
	}
}

bool AFarmPlot::Interact(AActor* InteractingActor)
{
	bool bSuccess = true;

	switch (FarmPlotState)
	{
	case EFarmPlotState::UnPlowed:
		FarmPlotState = EFarmPlotState::Plowed;
		break;
	case EFarmPlotState::Plowed:
		bSuccess = false;
		if (IInventoryInterface* InteractingInventory = Cast<IInventoryInterface>(InteractingActor))
		{
			if (UInventory* Inventory = InteractingInventory->GetInventory())
			{
				// Look for a seed in the interacting inventory
				// In the future the player should be able to pick what seed to plant
				const TArray<TObjectPtr<UInventoryItem>>& InventoryItems = Inventory->GetInventoryItems();
				for (const TObjectPtr<UInventoryItem> Item : InventoryItems)
				{
					if (Item)
					{
						if (USeedItemDefinition* SeedItemDef = Cast<USeedItemDefinition>(Item->ItemDefinition))
						{
							if (UPlantItemDefinition* PlantItemDef = Cast<UPlantItemDefinition>(SeedItemDef->PlantItemDef))
							{
								if (PlantChildActor.IsValid())
								{
									Inventory->RemoveInventoryItem(Item);
									PlantChildActor->SetPlantItemDef(PlantItemDef);
									PlantChildActor->SetGrowthPercentage(0.0f);

									GrowthTime = PlantItemDef->GrowthTime;
									FarmPlotState = EFarmPlotState::Growing;

									bSuccess = true;

									break;
								}
							}
						}
					}
				}	
			}
		}
		break;
	case EFarmPlotState::Growing:
		CurrentGrowTime = 0.0f;
		FarmPlotState = EFarmPlotState::UnPlowed;
		break;
	case EFarmPlotState::Grown:
		if (PlantChildActor.IsValid())
		{
			if (IInventoryInterface* InteractingInventory = Cast<IInventoryInterface>(InteractingActor))
			{
				const TArray<UItemDefinition*> HarvestedItems = PlantChildActor->Harvest();
				for (UItemDefinition* HarvestedItem : HarvestedItems)
				{
					InteractingInventory->AddInventoryItem(HarvestedItem);
				}

				CurrentGrowTime = 0.0f;
				FarmPlotState = EFarmPlotState::UnPlowed;
				break;
			}
		}
		bSuccess = false;
		break;
	default:
		break;
	}

	return bSuccess;
}

FString AFarmPlot::GetInteractText() const
{
	FString InteractText;

	switch (FarmPlotState)
	{
	case EFarmPlotState::UnPlowed:
		InteractText = TEXT("Plow");
		break;
	case EFarmPlotState::Plowed:
		InteractText = TEXT("Plant");
		break;
	case EFarmPlotState::Growing:
	case EFarmPlotState::Grown:
		InteractText = TEXT("Harvest");
		break;
	default:
		break;
	}

	return InteractText;
}

