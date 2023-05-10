// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "FarmPlot.generated.h"

class UChildActorComponent;
class AFarmPlant;
class UItemDefinition;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EFarmPlotState : uint8
{
	UnPlowed,
	Plowed,
	Growing,
	Grown
};

UCLASS(Blueprintable)
class RPG_TUTORIAL_API AFarmPlot : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	

	AFarmPlot();

protected:
	/** Updates the plant actor location */
	virtual void PostRegisterAllComponents() override;

	virtual void BeginPlay() override;

	/** Farm plot's static mesh */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	/** Farm plot's static mesh */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UChildActorComponent> PlantActorComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	/** State of the farm plot */
	UPROPERTY(BlueprintReadOnly)
	EFarmPlotState FarmPlotState = EFarmPlotState::UnPlowed;

	/** Item def of the seeds that can be planted in this plot.
	* Should eventually be replaced with something grabbed from a seed specific item def class*/
	UPROPERTY(Transient, BlueprintReadWrite)
	TWeakObjectPtr<AFarmPlant> PlantChildActor;

	/** How long it has been since the plant has started growing */
	UPROPERTY(BlueprintReadOnly)
	float CurrentGrowTime = 0.0f;

	/** How long it takes for the plant to finish growing */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GrowthTime = 60.0f;

public:
	//~=============================================================================
	// Interact Interface

	/** Attempts to bring farm to next state */
	UFUNCTION()
	virtual bool Interact(AActor* InteractingActor) override;

	/** Override to display custom pickup text when interacting */
	UFUNCTION()
	virtual FString GetInteractText() const;
};
