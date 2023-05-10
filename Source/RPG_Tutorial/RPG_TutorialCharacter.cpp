// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_TutorialCharacter.h"
#include "Camera/CameraComponent.h"
#include "CharacterStatsComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InteractInterface.h"
#include "ItemDefinition.h"
#include "Inventory.h"
#include "InventoryItem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

namespace RPGCharacterCVars
{
	static bool bDebugVault = false;
	static FAutoConsoleVariableRef CVarDebugVault(
		TEXT("RPGCharacter.Debug.Vault"),
		bDebugVault,
		TEXT("Draw debugging for vaulting mechanic"),
		ECVF_Default);

	static bool bDebugTargetTrace = false;
	static FAutoConsoleVariableRef CVarDebugTargetTrace(
		TEXT("RPGCharacter.Debug.TargetTrace"),
		bDebugVault,
		TEXT("Draw debugging for target tracking mechanic"),
		ECVF_Default);
};

//////////////////////////////////////////////////////////////////////////
// ARPG_TutorialCharacter

ARPG_TutorialCharacter::ARPG_TutorialCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Create interact area
	InteractSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractArea"));
	InteractSphereComponent->SetupAttachment(RootComponent);
	InteractSphereComponent->SetSphereRadius(125.f);
	InteractSphereComponent->SetRelativeLocation(FVector(90.f, 0.f, 0.f));

	StatsComponent = CreateDefaultSubobject<UCharacterStatsComponent>(TEXT("StatsComponent"));
}

void ARPG_TutorialCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (InteractSphereComponent)
	{
		InteractSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARPG_TutorialCharacter::OnInteractOverlapBegin);
		InteractSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ARPG_TutorialCharacter::OnInteractOverlapEnd);
	}
}

void ARPG_TutorialCharacter::BeginPlay()
{
	if (!Inventory)
	{
		Inventory = NewObject<UInventory>(this);
	}

	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ARPG_TutorialCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (InteractSphereComponent)
	{
		InteractSphereComponent->OnComponentBeginOverlap.RemoveAll(this);
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SprintUpdateHandle);
	}

	Super::EndPlay(EndPlayReason);
}

void ARPG_TutorialCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TargetLockActor.IsValid() && Controller)
	{
		const FVector TargetCameraLocation = GetActorLocation() + FVector(0.f, 0.f, TargetLockCameraHeightOffset);
		Controller->SetControlRotation(UKismetMathLibrary::FindLookAtRotation(TargetCameraLocation, TargetLockActor->GetActorLocation()));
	}

	UpdateInteractableActor();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARPG_TutorialCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPG_TutorialCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPG_TutorialCharacter::Look);

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ARPG_TutorialCharacter::ToggleCrouch);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ARPG_TutorialCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ARPG_TutorialCharacter::EndSprint);

		// Vaulting
		EnhancedInputComponent->BindAction(VaultAction, ETriggerEvent::Started, this, &ARPG_TutorialCharacter::Vault);

		// Target Lock
		EnhancedInputComponent->BindAction(TargetLockAction, ETriggerEvent::Started, this, &ARPG_TutorialCharacter::ToggleTargetLock);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ARPG_TutorialCharacter::Interact);
	}

}

void ARPG_TutorialCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARPG_TutorialCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARPG_TutorialCharacter::ToggleCrouch(const FInputActionValue& Value)
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

bool ARPG_TutorialCharacter::IsSprinting() const
{
	return SprintUpdateHandle.IsValid();
}

bool ARPG_TutorialCharacter::CanSprint() const
{
	return !bIsCrouched && StatsComponent->GetStamina() > 0.f;
}

void ARPG_TutorialCharacter::StartSprint(const FInputActionValue& Value)
{
	if (CanSprint())
	{
		if (UWorld* World = GetWorld())
		{
			CachedWalkingSpeed = GetCharacterMovement()->MaxWalkSpeed;
			GetCharacterMovement()->MaxWalkSpeed = SprintMovementSpeed;

			World->GetTimerManager().SetTimer(SprintUpdateHandle, this, &ARPG_TutorialCharacter::UpdateSprint, SprintUpdateRate, true);
		}
	}
}

void ARPG_TutorialCharacter::UpdateSprint()
{
	if (CanSprint())
	{
		if (GetVelocity().Length() > 0.0f && GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
		{
			// If stamina hits 0 after decreasing, end the sprint
			if (StatsComponent->DecreaseStamina(SprintingStaminaDecreaseRate * SprintUpdateRate))
			{
				EndSprint();
			}
		}
	}
	else
	{
		EndSprint();
	}
}

void ARPG_TutorialCharacter::EndSprint(const FInputActionValue& Value)
{
	if (IsSprinting())
	{
		if (UWorld* World = GetWorld())
		{
			GetCharacterMovement()->MaxWalkSpeed = CachedWalkingSpeed;
			World->GetTimerManager().ClearTimer(SprintUpdateHandle);
		}
	}
}

void ARPG_TutorialCharacter::Vault(const FInputActionValue& Value)
{
	// Sphere trace forward at different heights to find a wall that may be able to be vaulted over
	for (int32 ForwardTraceIndex = 0;  ForwardTraceIndex < VaultForwardTraceCount;  ForwardTraceIndex++)
	{
		const FVector ForwardTraceStart = GetActorLocation() + FVector(0.f, 0.f, VaultForwardTraceSpacing * ForwardTraceIndex);
		const FVector ForwardTraceEnd = ForwardTraceStart + (GetActorForwardVector() * VaultForwardTraceDistance);
		TArray<AActor*> ActorsToIgnore;
		FHitResult ForwardTraceHit;
		const bool bIgnoreSelf = true;
		const EDrawDebugTrace::Type DebugTrace = RPGCharacterCVars::bDebugVault ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		if (UKismetSystemLibrary::SphereTraceSingle(this, ForwardTraceStart, ForwardTraceEnd, VaultForwardTraceRadius, ETraceTypeQuery::TraceTypeQuery3, false, ActorsToIgnore, DebugTrace, ForwardTraceHit, bIgnoreSelf))
		{
			// Declaring the following locations to be set while querying the vaulting surface
			//	Location to jump towards when starting the vault
			FVector VaultStartPos = FVector::ZeroVector;

			//	Last location on the surface to vault over where the character launches off the surface
			FVector VaultLaunchPos = FVector::ZeroVector;

			//	Location to land at after launching of the surface
			FVector VaultEndPos = FVector::ZeroVector;

			// Check if the found wall can be vaulted over by doing a series of traces downwards to find the top surface, and how far forward that surface goes
			const FVector DistanceTraceReferencePoint = ForwardTraceHit.Location + FVector(0.0f, 0.0f, VaultDistanceTraceStartHeight);
			for (int32 DistanceTraceIndex = 0; DistanceTraceIndex < VaultDistanceTraceCount; DistanceTraceIndex++)
			{
				const FVector DistanceTraceStart = DistanceTraceReferencePoint + (GetActorForwardVector() * VaultDistanceTraceSpacing * DistanceTraceIndex);
				const FVector DistanceTraceEnd = DistanceTraceStart - FVector(0.f, 0.f, VaultDistanceTraceMaxDepth);
				FHitResult DistanceTraceHit;
				if (UKismetSystemLibrary::SphereTraceSingle(this, DistanceTraceStart, DistanceTraceEnd, VaultDistanceTraceRadius, ETraceTypeQuery::TraceTypeQuery3, false, ActorsToIgnore, DebugTrace, DistanceTraceHit, bIgnoreSelf))
				{
					// If the trace did not start inside of the wall
					if (DistanceTraceHit.IsValidBlockingHit())
					{
						// If this is the first trace, the hit point represents the closest edge of the surface to vault over
						if (DistanceTraceIndex == 0)
						{
							VaultStartPos = DistanceTraceHit.Location;

							if (RPGCharacterCVars::bDebugVault)
							{
								DrawDebugSphere(GetWorld(), VaultStartPos, 8.f, 12, FColorList::LimeGreen);
							}
						}

						// Keep updating the launch position until there is no more surface to vault over
						VaultLaunchPos = DistanceTraceHit.Location;

						if (RPGCharacterCVars::bDebugVault)
						{
							DrawDebugSphere(GetWorld(), VaultLaunchPos, 5.f, 12, FColorList::Yellow);
						}
					}
				}
				// Find landing location if the start and middle of the vault are valid
				else if (VaultStartPos != FVector::ZeroVector && VaultLaunchPos != FVector::ZeroVector)
				{
					// Clamp landing location to a minimum distance from the start of the animation
					const float VaultDistance = FVector::Dist2D(GetActorLocation(), DistanceTraceStart);
					const float ToReachMinimumDistance = VaultMinTotalDistance - VaultDistance;
					const float LandingTraceStartDist = FMath::Max(VaultLandingTraceStartDistance, ToReachMinimumDistance);

					const FVector LandingTraceStart = DistanceTraceStart + (GetActorForwardVector() * LandingTraceStartDist);
					const FVector LandingTraceEnd = LandingTraceStart - FVector(0.f, 0.f, VaultLandingMaxHeight);
					FHitResult LandingTraceHit;
					if (UKismetSystemLibrary::LineTraceSingle(this, LandingTraceStart, LandingTraceEnd, ETraceTypeQuery::TraceTypeQuery3, false, ActorsToIgnore, DebugTrace, LandingTraceHit, bIgnoreSelf))
					{
						VaultEndPos = LandingTraceHit.Location;

						if (RPGCharacterCVars::bDebugVault)
						{
							DrawDebugSphere(GetWorld(), VaultEndPos, 5.f, 12, FColorList::VioletRed);
						}

						// Vault! Have blueprint handling vaulting animation
						OnVault(VaultStartPos, VaultLaunchPos, VaultEndPos);
						return;
					}
				}
			}
		}
	}
}

void ARPG_TutorialCharacter::ToggleTargetLock(const FInputActionValue& Value)
{
	if (TargetLockActor.IsValid())
	{
		TargetLockActor = nullptr;
	}
	else
	{
		const FVector TargetLockTraceStart = FollowCamera->GetComponentLocation();
		const FVector TargetLockTraceEnd = TargetLockTraceStart + (FollowCamera->GetForwardVector() * TargetLockDetectionDistance);
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);
		TArray<AActor*> ActorsToIgnore;
		FHitResult TargetLockTraceHit;
		const bool bIgnoreSelf = true;
		const EDrawDebugTrace::Type DebugTrace = RPGCharacterCVars::bDebugTargetTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		if (UKismetSystemLibrary::SphereTraceSingleForObjects(this, TargetLockTraceStart, TargetLockTraceEnd, TargetLockDetectionRadius, ObjectTypes, false, ActorsToIgnore, DebugTrace, TargetLockTraceHit, bIgnoreSelf))
		{
			if (const AActor* FoundActor = TargetLockTraceHit.GetActor())
			{
				if (FoundActor->ActorHasTag(TEXT("Attackable")))
				{
					TargetLockActor = FoundActor;
				}
			}
		}
	}
}

void ARPG_TutorialCharacter::Interact(const FInputActionValue& Value)
{
	if (InteractableInterface && InteractableInterface->CanInteract(this))
	{
		InteractableInterface->Interact(this);
	}
}

float ARPG_TutorialCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float AppliedDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (StatsComponent->DecreaseHealth(AppliedDamage))
	{
		Die();
	}

	return AppliedDamage;
}

void ARPG_TutorialCharacter::Die()
{
	DisableInput(nullptr);

	// Rag-doll
	GetMesh()->SetSimulatePhysics(true);
}

UInventory* ARPG_TutorialCharacter::GetInventory() const
{
	return Inventory;
}

UInventoryItem* ARPG_TutorialCharacter::GetEquippedItem() const
{
	return EquippedItem;
}

void ARPG_TutorialCharacter::EquipItem(UInventoryItem* ItemToEquip)
{
	if (ItemToEquip)
	{
		if (const UItemDefinition* ItemDef = ItemToEquip->ItemDefinition)
		{
			if (ItemDef->bCanBeEquipped)
			{
				EquippedItem = ItemToEquip;
				OnItemEquipped(EquippedItem);
			}
		}
	}
}

void ARPG_TutorialCharacter::OnInteractOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IInteractInterface* InteractInterface = Cast<IInteractInterface>(OtherActor))
	{
		InteractableActors.Add(OtherActor);
	}
}

void ARPG_TutorialCharacter::OnInteractOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	InteractableActors.Remove(OtherActor);
}

void ARPG_TutorialCharacter::SetInteractableInterface(IInteractInterface* NewInteractableInterface)
{
	InteractableInterface = NewInteractableInterface;
	OnInteractableActorSet(Cast<AActor>(InteractableInterface));
}

void ARPG_TutorialCharacter::UpdateInteractableActor()
{
	// Choose the closest interactable actor
	float MinInteractableDistance = FLT_MAX;
	const FVector CharacterLocation = GetActorLocation();
	IInteractInterface* ClosestInteractable = nullptr;
	for (int32 i = InteractableActors.Num() - 1; i >= 0; --i)
	{
		TWeakObjectPtr<AActor> InteractableActor = InteractableActors[i];
		if (InteractableActor.IsValid())
		{
			const float DistFromCharacter = FVector::Dist2D(CharacterLocation, InteractableActor->GetActorLocation());
			if (DistFromCharacter < MinInteractableDistance)
			{
				ClosestInteractable = Cast<IInteractInterface>(InteractableActor.Get());

				// Double check that the actor implements IInteractInterface
				if (ClosestInteractable)
				{
					MinInteractableDistance = DistFromCharacter;
				}
				else
				{
					InteractableActors.RemoveAt(i);
				}
			}
		}
		else
		{
			InteractableActors.RemoveAt(i);
		}
	}

	SetInteractableInterface(ClosestInteractable);
}