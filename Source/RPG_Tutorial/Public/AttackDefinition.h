// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttackDefinition.generated.h"

class UAnimMontage;

/**
 * Defines an attack
 */
UCLASS(Blueprintable)
class RPG_TUTORIAL_API UAttackDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/** How much damage the attach applies */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage = 10.0f;

	/** Item's UI representation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AnimMontage;

	/** Gameplay effect that occurs when attack hits */
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TObjectPtr<UGameplayEffect> Mesh;
};
