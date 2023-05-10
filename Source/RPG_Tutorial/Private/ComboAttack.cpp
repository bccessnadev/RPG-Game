// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboAttack.h"

#include "AttackDefinition.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY(LogComboSystem);

UComboAttack* UComboAttack::CreateComboAttack(const TArray<FAttack_Combo>& Attacks, ACharacter* Character)
{
	UComboAttack* NewComboAttack = NewObject<UComboAttack>(Character);
	NewComboAttack->Attacks = Attacks;
	NewComboAttack->CachedCharacter = Character;

	return NewComboAttack;
}

const TArray<FAttack_Combo>& UComboAttack::GetAttacks()
{
	return Attacks;
}

FAttack_Combo UComboAttack::GetNextAttack() const
{
	const int32 NextAttackIndex = CurrentAttackIndex + 1;
	if (Attacks.IsValidIndex(NextAttackIndex))
	{
		return Attacks[CurrentAttackIndex];
	}

	return FAttack_Combo();
}

bool UComboAttack::TryAttack(UInputAction* Input)
{
	const int32 NextAttackIndex = CurrentAttackIndex + 1;
	if (Input && CachedCharacter.IsValid() && Attacks.IsValidIndex(NextAttackIndex) && Attacks[NextAttackIndex].InputAction == Input)
	{
		if (NextAttackIndex == 0)
		{
			StartNextAttack();
		}
		else
		{
			bNextAttackQueued = true;
		}

		return true;
	}

	return false;
}

void UComboAttack::TryStartNextAttack()
{
	if (bNextAttackQueued)
	{
		StartNextAttack();
	}
}

void UComboAttack::CancelCombo()
{
	CurrentAttackIndex = -1;
	bNextAttackQueued = false;
}

bool UComboAttack::IsComboActive()
{
	return CurrentAttackIndex >= 0;
}

void UComboAttack::StartNextAttack()
{
	const int32 NextAttackIndex = CurrentAttackIndex + 1;
	if (CachedCharacter.IsValid() && Attacks.IsValidIndex(NextAttackIndex))
	{
		UE_LOG(LogComboSystem, Log, TEXT("Starting attack [%i]"), NextAttackIndex);
		FAttack_Combo& NextAttack = Attacks[NextAttackIndex];
		CurrentAttackIndex = NextAttackIndex;
		bNextAttackQueued = false;

		if (UAttackDefinition* AttackDef = NextAttack.AttackDefinition)
		{
			float AnimLength = CachedCharacter->PlayAnimMontage(AttackDef->AnimMontage, NextAttack.AnimationSpeedMultiplier);
		}
	}
}