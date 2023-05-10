// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_TUTORIAL_API IInteractInterface
{
	GENERATED_BODY()

public:
	/** Whether the object can be interacted with */
	UFUNCTION(BlueprintCallable)
	virtual bool CanInteract(const AActor* InteractingActor) const { return true; }

	/** Interact with the object. Returns true if successful */
	UFUNCTION(BlueprintCallable)
	virtual bool Interact(AActor* InteractingActor) = 0;

	/** Returns the text that displays when interact is available */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FString GetInteractText() const { return TEXT("Interact"); }
};
