// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ComboAttack.generated.h"

class UAttackDefinition;
class ACharacter;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogComboSystem, Log, All);

USTRUCT(Blueprintable)
struct FAttack_Combo
{
	GENERATED_BODY()

	/** Definition of the attack */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAttackDefinition> AttackDefinition;

	/** Input action required to trigger this attack */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> InputAction;

	/** How much more or less damage the attack does in this combo */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DamageMultiplier = 1.f;

	/** Multiplier for attack animation speed during this combo */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AnimationSpeedMultiplier = 1.f;
};

/**
 * 
 */
UCLASS(Blueprintable)
class RPG_TUTORIAL_API UComboAttack : public UObject
{
	GENERATED_BODY()

public:
	/** Creates a new Combo Attack with the given attacks */
	UFUNCTION(BlueprintCallable)
	static UComboAttack* CreateComboAttack(const TArray<FAttack_Combo>& Attacks, ACharacter* Character);
	
	/** Returns the attacks that make up this combo */
	UFUNCTION(BlueprintPure)
	const TArray<FAttack_Combo>& GetAttacks();

	/** Returns the next attack in the combo */
	UFUNCTION(BlueprintPure)
	FAttack_Combo GetNextAttack() const;

	/** Attempts to start or continue the combo with the given input action */
	UFUNCTION(BlueprintCallable)
	bool TryAttack(UInputAction* Input);

	/** Attempts to start the next attack if a input for it has been previously given
	* Should be called by the previous attack's AnimNotify */
	UFUNCTION(BlueprintCallable)
	void TryStartNextAttack();

	/** Cancels the attack combo */
	UFUNCTION(BlueprintCallable)
	void CancelCombo();

	/** Returns true if the combo currently in progress */
	UFUNCTION(BlueprintCallable)
	bool IsComboActive();

protected:
	/** Starts the animation of the next attack and set's it as the current */
	UFUNCTION(BlueprintCallable)
	void StartNextAttack();

	/** The attacks that make up this combo */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetAttacks)
	TArray<FAttack_Combo> Attacks;

	/** The index of the current active attack */
	UPROPERTY(Transient)
	int32 CurrentAttackIndex = -1;

	/** Should the next attack start once the current attack's AnimNotify triggers */
	UPROPERTY(Transient)
	bool bNextAttackQueued = false;

	/** Character that is performing the attack. Needed to play the attack animation */
	UPROPERTY(Transient)
	TWeakObjectPtr<ACharacter> CachedCharacter;
};
