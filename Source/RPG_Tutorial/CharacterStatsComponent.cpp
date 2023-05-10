// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatsComponent.h"

// Sets default values for this component's properties
UCharacterStatsComponent::UCharacterStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

float UCharacterStatsComponent::GetHealth() const
{
	return Health;
}

float UCharacterStatsComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UCharacterStatsComponent::GetStamina() const
{
	return Stamina;
}

float UCharacterStatsComponent::GetMaxStamina() const
{
	return MaxStamina;
}

float UCharacterStatsComponent::GetXP() const
{
	return XP;
}

float UCharacterStatsComponent::GetMaxXP() const
{
	return MaxXP;
}

int32 UCharacterStatsComponent::GetLevel() const
{
	return Level;
}

bool UCharacterStatsComponent::IncreaseHealth(const float HealthIncrease)
{
	Health = FMath::Min<float>(Health + HealthIncrease, MaxHealth);
	OnHealthUpdated();
	return Health == MaxHealth;
}

bool UCharacterStatsComponent::DecreaseHealth(const float HealthDecrease)
{
	Health = FMath::Max<float>(Health - HealthDecrease, 0.0f);
	OnHealthUpdated();
	return Health == 0.0f;
}

void UCharacterStatsComponent::SetHealth(const float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealthUpdated();
}

bool UCharacterStatsComponent::IncreaseStamina(const float StaminaIncrease)
{
	Stamina = FMath::Min<float>(Stamina + StaminaIncrease, MaxStamina);
	OnStaminaUpdated();
	return Stamina == MaxStamina;
}

bool UCharacterStatsComponent::DecreaseStamina(const float StaminaDecrease)
{
	Stamina = FMath::Max<float>(Stamina - StaminaDecrease, 0.0f);
	OnStaminaUpdated();
	return Stamina == 0.0f;
}

void UCharacterStatsComponent::SetStamina(const float NewStamina)
{
	Stamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);
	OnStaminaUpdated();
}

bool UCharacterStatsComponent::IncreaseXP(const float XPIncrease)
{
	XP += XPIncrease;

	// Once character has reached max XP of current level, increase level
	bool bLeveledUp = false;
	if (XP > MaxXP)
	{
		IncreaseLevel(1);
		bLeveledUp = true;
	}

	OnXPUpdated();

	return bLeveledUp;
}

void UCharacterStatsComponent::IncreaseLevel(const int32 LevelIncrease)
{
	Level += LevelIncrease;
	
	// Reset the XP now the character's level has increased
	XP -= MaxXP;

	// Increase the Max XP for the new level
	// This is a temporary calculation until data based system is implemented
	//	Increase the max XP needed to level by 50 for each level
	MaxXP += LevelIncrease * 50.f;

	OnLevelUpdated();
}

void UCharacterStatsComponent::OnHealthUpdated()
{
	HealthUpdatedDelegate.Broadcast(Health, MaxHealth);
}

void UCharacterStatsComponent::OnStaminaUpdated()
{
	StaminaUpdatedDelegate.Broadcast(Stamina, MaxStamina);
}

void UCharacterStatsComponent::OnXPUpdated()
{
	XPUpdatedDelegate.Broadcast(XP, MaxXP);
}

void UCharacterStatsComponent::OnLevelUpdated()
{
	LevelUpdatedDelegate.Broadcast(Level, 0.0f);
}

