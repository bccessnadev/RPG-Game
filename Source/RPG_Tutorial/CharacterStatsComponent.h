// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatUpdated, float, CurrentStatValue, float, MaxStatValue);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_TUTORIAL_API UCharacterStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatsComponent();

	/** Returns current health of character */
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetHealth() const;

	/** Returns current health of character */
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetMaxHealth() const;

	/** Returns current stamina of character */
	UFUNCTION(BlueprintCallable, Category = Stamina)
	float GetStamina() const;

	/** Returns current stamina of character */
	UFUNCTION(BlueprintCallable, Category = Stamina)
	float GetMaxStamina() const;

	/** Returns current experience of character */
	UFUNCTION(BlueprintCallable, Category = XP)
	float GetXP() const;

	/** Returns current experience of character */
	UFUNCTION(BlueprintCallable, Category = XP)
	float GetMaxXP() const;

	/** Returns current level of character */
	UFUNCTION(BlueprintCallable, Category = XP)
	int32 GetLevel() const;

	/** Increases the characters health by the given amount 
	* Returns true if character reached max health after increase */
	UFUNCTION(BlueprintCallable, Category = Health)
	bool IncreaseHealth(const float HealthIncrease);

	/** Decreases the characters health by the given amount 
	* Returns true if character's health reached 0 after decrease */
	UFUNCTION(BlueprintCallable, Category = Health)
	bool DecreaseHealth(const float HealthDecrease);

	/** Decreases the characters health by the given amount */
	UFUNCTION(BlueprintCallable, Category = Health)
	void SetHealth(const float NewHealth);

	/** Increases the characters stamina by the given amount 
	* Returns true if character reached max stamina after increase */
	UFUNCTION(BlueprintCallable, Category = Stamina)
	bool IncreaseStamina(const float StaminaIncrease);

	/** Decreases the characters stamina by the given amount 
	* Returns true if character's stamina reached 0 after decrease */
	UFUNCTION(BlueprintCallable, Category = Stamina)
	bool DecreaseStamina(const float StaminaDecrease);

	/** Decreases the characters stamina by the given amount */
	UFUNCTION(BlueprintCallable, Category = Stamina)
	void SetStamina(const float NewStamina);

	/** Increases the characters experience by the given amount 
	* Returns true if character leveled up after increase*/
	UFUNCTION(BlueprintCallable, Category = XP)
	bool IncreaseXP(const float XPIncrease);

	/** Increases the characters level by the given amount */
	UFUNCTION(BlueprintCallable, Category = XP)
	void IncreaseLevel(const int32 LevelIncrease);

	/** Broadcasts when characters current health is updated */
	UPROPERTY(BlueprintAssignable)
	FOnStatUpdated HealthUpdatedDelegate;

	/** Broadcasts when characters current stamina is updated */
	UPROPERTY(BlueprintAssignable)
	FOnStatUpdated StaminaUpdatedDelegate;

	/** Broadcasts when characters current experience is updated */
	UPROPERTY(BlueprintAssignable)
	FOnStatUpdated XPUpdatedDelegate;

	/** Broadcasts when characters current level is updated */
	UPROPERTY(BlueprintAssignable)
	FOnStatUpdated LevelUpdatedDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Called when value of current health changes */
	void OnHealthUpdated();

	/** Called when value of current stamina changes */
	void OnStaminaUpdated();

	/** Called when value of current experience changes */
	void OnXPUpdated();

	/** Called when value of current level changes */
	void OnLevelUpdated();

	/** Current health of the character*/
	UPROPERTY(BlueprintReadWrite, BlueprintGetter = GetHealth, Category = Health)
	float Health = 100.f;

	/** Max health of the character */
	UPROPERTY(BlueprintReadWrite, BlueprintGetter = GetMaxHealth, Category = Health)
	float MaxHealth = 100.f;

	/** Current stamina of the character*/
	UPROPERTY(BlueprintReadWrite, BlueprintGetter = GetStamina, Category = Stamina)
	float Stamina = 100.f;

	/** Max stamina of the character */
	UPROPERTY(BlueprintReadWrite, BlueprintGetter = GetMaxStamina, Category = Stamina)
	float MaxStamina = 100.f;

	/** Current experience of the character */
	UPROPERTY(BlueprintReadWrite, BlueprintGetter = GetXP, Category = XP)
	float XP = 0.f;

	/** Total amount of experience needed for character to reach next level */
	UPROPERTY(BlueprintReadWrite, BlueprintGetter = GetMaxXP, Category = XP)
	float MaxXP = 100.f;

	/** Total amount of experience needed for character to reach next level */
	UPROPERTY(BlueprintReadWrite, BlueprintGetter = GetLevel, Category = XP)
	int32 Level = 0;
		
};
