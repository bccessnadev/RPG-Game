// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InventoryInterface.h"
#include "RPG_TutorialCharacter.generated.h"

class UCharacterStatsComponent;
class UInputAction;
class IInteractInterface;

UCLASS(config=Game)
class ARPG_TutorialCharacter : public ACharacter, public IInventoryInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Stats component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCharacterStatsComponent* StatsComponent;
	
	/** Sphere area in which character can interact with objects */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* InteractSphereComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* VaultAction;

	/** Target Lock Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TargetLockAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

public:
	ARPG_TutorialCharacter();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Bind to Component events */
	virtual void PostInitializeComponents() override;

	// To add mapping context
	virtual void BeginPlay();

	/** Cleanup */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Handles per frame logic */
	virtual void Tick(float DeltaSeconds) override;

public:
	/** Returns true if the character is sprinting */
	UFUNCTION(BlueprintCallable, Category = Sprinting)
	bool IsSprinting() const;

	/** Returns true if the character can sprint */
	UFUNCTION(BlueprintCallable, Category = Sprinting)
	bool CanSprint() const;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	/** Called for crouch input */
	void ToggleCrouch(const FInputActionValue& Value);

	/** Called for sprint start input */
	void StartSprint(const FInputActionValue& Value);

	/** Called on timer to update active sprint */
	void UpdateSprint();

	/** Called for sprint end input */
	void EndSprint(const FInputActionValue& Value = FInputActionValue());

	/** Called for vault input */
	void Vault(const FInputActionValue& Value);

	/** Blueprint function for playing vaulting montage */
	UFUNCTION(BlueprintImplementableEvent)
	void OnVault(const FVector& VaultStartPos, const FVector& VaultLaunchPos, const FVector& VaultLandPos);

	/** Called for Target Lock input */
	void ToggleTargetLock(const FInputActionValue& Value);

	/** Called for interact input */
	void Interact(const FInputActionValue& Value);

	/** Timer handle for sprint update */
	FTimerHandle SprintUpdateHandle;

	/** Characters movement speed while sprinting */
	UPROPERTY(BlueprintReadWrite, Category = Sprinting)
	float SprintMovementSpeed = 750.f;

	/** How much stamina is lost per second while sprinting */
	UPROPERTY(BlueprintReadWrite, Category = Sprinting)
	float SprintingStaminaDecreaseRate = 10.f;

	/** How often the UpdateSprint function gets called while sprinting */
	UPROPERTY(EditDefaultsOnly, Category = Sprinting)
	float SprintUpdateRate = 0.1f;

	/** Cached value of max walking speed
	* Allows for returning to the previous walking speed after modifying it */
	UPROPERTY(Transient)
	float CachedWalkingSpeed = 0.f;

	/** How many traces to perform to detect a vaulting location in front of the player */
	UPROPERTY(EditDefaultsOnly, Category = Vaulting)
	int32 VaultForwardTraceCount = 3;

	/** Z distance between vaulting forward traces */
	UPROPERTY(EditDefaultsOnly, Category = Vaulting)
	float VaultForwardTraceSpacing = 30.f;

	/** Radius of the vaulting forward trace*/
	UPROPERTY(EditDefaultsOnly, Category = Vaulting)
	float VaultForwardTraceRadius = 5.f;

	/** How far in front of the character the vaulting forward trace reaches */
	UPROPERTY(EditDefaultsOnly, Category = Vaulting)
	float VaultForwardTraceDistance = 200.f;

	/** How many traces to perform to detect the distance of the vault over a surface */
	UPROPERTY(EditDefaultsOnly, Category = Vaulting)
	int32 VaultDistanceTraceCount = 5;

	/** Distance between vaulting distance detection traces */
	UPROPERTY(EditDefaultsOnly, Category = Vaulting)
	float VaultDistanceTraceSpacing = 50.f;

	/** Radius of the vaulting distance trace*/
	UPROPERTY(EditDefaultsOnly, Category = Vaulting)
	float VaultDistanceTraceRadius = 8.f;

	/** How high above the found vaulting point in front of the player to start checking for a distance to vault over */
	UPROPERTY(EditDefaultsOnly, Category = Vaulting)
	float VaultDistanceTraceStartHeight = 100.f;

	/** How far down the vaulting distance trace checks for the surface to vault over */
	UPROPERTY(EditDefaultsOnly, Category = Vaulting)
	float VaultDistanceTraceMaxDepth = 150.f;

	/** Minimum distance a vault should move the player. Helps avoid awkward animation when distance is too short */
	UPROPERTY(EditDefaultsOnly, Category = Vaulting)
	float VaultMinTotalDistance = 300.f;

	/** How far away from the end of the surface to vault off to find a point to land */
	UPROPERTY(EditDefaultsOnly, Category = Vaulting)
	float VaultLandingTraceStartDistance = 80.f;

	/** How far down to check for a place to land after vaulting */
	UPROPERTY(EditDefaultsOnly, Category = Vaulting)
	float VaultLandingMaxHeight = 1000.f;

	/** Actor that character the character's camera follows */
	TWeakObjectPtr<const AActor> TargetLockActor = nullptr;

	/** How far a target lock actor can be selected from */
	UPROPERTY(EditDefaultsOnly, Category = TargetLock)
	float TargetLockDetectionDistance = 1000.f;

	/** Radius of the trace from the camera to detect actors for target lock */
	UPROPERTY(EditDefaultsOnly, Category = TargetLock)
	float TargetLockDetectionRadius = 150.f;

	/** Radius of the trace from the camera to detect actors for target lock */
	UPROPERTY(EditDefaultsOnly, Category = TargetLock)
	float TargetLockCameraHeightOffset = 100.f;

protected:
	/** Sends damage to stats component */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** Handles character death */
	void Die();

public:
	/** Returns the character's inventory*/
	virtual UInventory* GetInventory() const override;

	/** Returns the item the player has currently equiped */
	UFUNCTION(BlueprintCallable)
	UInventoryItem* GetEquippedItem() const;

	/** Returns the static mesh component of the characters held item */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UStaticMeshComponent* GetEquipedItemMesh() const;

	/** Sets the item the player has currently equiped */
	UFUNCTION(BlueprintCallable)
	void EquipItem(UInventoryItem* ItemToEquip);

protected:
	/** Collection of character's items */
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UInventory> Inventory;

	/** Collection of character's items */
	UPROPERTY(BlueprintGetter = GetEquippedItem, BlueprintSetter = EquipItem)
	TObjectPtr<UInventoryItem> EquippedItem;

	/** Allows BP to implement equipped item cosmetics */
	UFUNCTION(BlueprintImplementableEvent)
	void OnItemEquipped(UInventoryItem* Item);

protected:
	/** Tracks interactable objects as the enter the character's interact area */
	UFUNCTION()
	void OnInteractOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Tracks interactable objects as the exit the character's interact area */
	UFUNCTION()
	void OnInteractOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Updates the intractable interface and notifies the BP */
	void SetInteractableInterface(IInteractInterface* NewInteractableInterface);

	/** Allows the BP to implement visuals for the intractable item */
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractableActorSet(AActor* InteractableActor);

	/** Chooses the most relevant interactable actor from InteractableActors 
	* Called on tick */
	void UpdateInteractableActor();

	/** Actors within the character's interact range. One of these are picked to be the InteractableInterface */
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AActor>> InteractableActors;

	/** Actor to interact with */
	IInteractInterface* InteractableInterface = nullptr;
};

