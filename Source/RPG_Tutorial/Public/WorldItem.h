// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "WorldItem.generated.h"

class UItemDefinition;
class UStaticMeshCompoent;

UCLASS(Blueprintable)
class RPG_TUTORIAL_API AWorldItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldItem();

	/** Updates the world item's mesh when the item definition is set */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/** Returns the item definition */
	UFUNCTION(BlueprintPure)
	UItemDefinition* GetItemDefinition() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Definition for this item */
	UPROPERTY(EditAnywhere, BlueprintGetter = GetItemDefinition)
	TObjectPtr<UItemDefinition> ItemDefinition;

	/** Item's 3D representation */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

public:
	//~=============================================================================
	// Interact Interface

	/** Attempts to add item to the interacting actor's inventory */
	UFUNCTION()
	virtual bool Interact(AActor* InteractingActor) override;

	/** Override to display custom pickup text when interacting */
	UFUNCTION()
	virtual FString GetInteractText() const {return InteractText; }

protected:

	/** Item's 3D representation */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString InteractText;
};
