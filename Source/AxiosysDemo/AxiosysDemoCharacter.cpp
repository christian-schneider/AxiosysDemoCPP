// Copyright Epic Games, Inc. All Rights Reserved.

#include "AxiosysDemoCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AxiosysDemo.h"
#include "AxiosysMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

AAxiosysDemoCharacter::AAxiosysDemoCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UAxiosysMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	AxiosysMovementComponent = Cast<UAxiosysMovementComponent>(GetCharacterMovement());
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
	
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true ;
}

void AAxiosysDemoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AAxiosysDemoCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AAxiosysDemoCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAxiosysDemoCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAxiosysDemoCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AAxiosysDemoCharacter::LookInput);
		
		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AAxiosysDemoCharacter::DoCrouchStart);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AAxiosysDemoCharacter::DoCrouchEnd);
		
		// Parkour
		EnhancedInputComponent->BindAction(ParkourAction, ETriggerEvent::Started, this, &AAxiosysDemoCharacter::DoParkourStart);
		EnhancedInputComponent->BindAction(ParkourAction, ETriggerEvent::Completed, this, &AAxiosysDemoCharacter::DoParkourEnd);
	}
	else
	{
		UE_LOG(LogAxiosysDemo, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}




void AAxiosysDemoCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void AAxiosysDemoCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void AAxiosysDemoCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AAxiosysDemoCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AAxiosysDemoCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AAxiosysDemoCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}


//  Crouching -------------------------------

void AAxiosysDemoCharacter::DoCrouchStart()
{
	UE_LOG(LogAxiosysDemo, Display, TEXT("Do Crouch"));
}

void AAxiosysDemoCharacter::DoCrouchEnd()
{
	UE_LOG(LogAxiosysDemo, Display, TEXT("Do Crouch - End"));
}


//  Parkour -------------------------------

void AAxiosysDemoCharacter::DoParkourStart()
{
	bool bDrawDebugLines = false;
	if (!bCanParkour) {
		return; 
	}
	bCanParkour = false;
	
	UE_LOG(LogAxiosysDemo, Display, TEXT("Do Parkour - Start"));
	
	// Check if we are in front of a parkourable object
	
	FVector StartLocation = GetActorLocation() - FVector(0.0f, 0.0f, 55.0f);
	FVector EndLocation = GetActorForwardVector() * 70.0f + StartLocation;
	FHitResult HitResult;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	if (GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, ObjectParams, Params))
	{
		// we are in front of a parkourable object
		
		if (bDrawDebugLines)
		{
			DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, 5.0f, 0, 5.0f);
		}
		
		// get wall location and wall normal
		FVector WallLocation = HitResult.Location;
		FVector WallNormal = HitResult.Normal;
		
		FVector WallHeight;
		FVector WallHeight2; // to test wall thickness
		
		bool bShouldClimb = false;
		bool bIsWallThick = true;
		
		// get wall height
		
		EndLocation = WallLocation + UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotFromX(WallNormal) * -10.0f);
		StartLocation = EndLocation + FVector(0.0f, 0.0f, 200.0f);
		if (GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, ObjectParams, Params))
		{
			if (bDrawDebugLines)
			{
				DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, 5.0f, 0, 5.0f);
			}
			WallHeight = HitResult.Location;
			bShouldClimb = (WallHeight - WallLocation).Z > 80.0f;
			
			EndLocation = WallLocation + UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotFromX(WallNormal) * -50.0f);
			StartLocation = EndLocation + FVector(0.0f, 0.0f, 250.0f);
			
			// check wall thickness
			
			if (GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, ObjectParams, Params))
			{
				if (bDrawDebugLines)
				{
					DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, 5.0f, 0, 5.0f);
				}
				WallHeight2 = HitResult.Location;
				bIsWallThick = ((WallHeight - WallHeight2).Z < 30.0f);
			} 
			else
			{
				bIsWallThick = false;
			}
			
			if (bShouldClimb)
			{
				// Climb up and over	
				
				// check if something is blocking the climbing
				StartLocation = GetActorLocation() + FVector(0.0f, 0.0f, 200.0f);
				EndLocation = StartLocation * 70.0f;
				if (GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, ObjectParams, Params))
				{
					bCanParkour = true;
				}
				else
				{
					GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					GetCharacterMovement()->SetMovementMode(MOVE_Flying);
					
					float WallZ = WallHeight.Z - 44.0f;
					FVector NewLocation = GetActorLocation();
					NewLocation.Z += WallZ;
					SetActorLocation(NewLocation);
					
					FTimerHandle ClimbTimerHandle; 
					float fTimerDelay = PlayAnimMontage(ClimbMontage);
					if (bIsWallThick)
					{
						
					}
					GetWorldTimerManager().SetTimer(ClimbTimerHandle, this, &AAxiosysDemoCharacter::RestoreCollisionAndMovement, fTimerDelay);
				}
			} 
			else
			{
				// Vault and get up
				UE_LOG(LogTemp, Warning, TEXT("HitActor = %s"), *HitResult.GetActor()->GetName());
				
				GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				GetCharacterMovement()->SetMovementMode(MOVE_Flying);
				
				FTimerHandle VaultTimerHandle; 
				float fTimerDelay = 0.0f;
				if (bIsWallThick)
				{
					FVector NewLocation =  GetActorLocation() + UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotFromX(WallNormal) * 50.0f);
					SetActorLocation(NewLocation);
					fTimerDelay = PlayAnimMontage(GettingUpMontage);
				}
				else
				{
					FVector NewLocation = GetActorLocation() - FVector(0.0f, 0.0f, WallHeight.Z);
					SetActorLocation(NewLocation);
					fTimerDelay = PlayAnimMontage(VaultMontage);
				}
				GetWorldTimerManager().SetTimer(VaultTimerHandle, this, &AAxiosysDemoCharacter::RestoreCollisionAndMovement, fTimerDelay);
			}
		}
		else
		{
			bCanParkour = true;
		}
	}
	else
	{
		bCanParkour = true;
	}
	// DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, 5.0f, 0, 5.0f);
}

void AAxiosysDemoCharacter::DoParkourEnd()
{
	
	UE_LOG(LogAxiosysDemo, Display, TEXT("Do Parkour - End"));
}

void AAxiosysDemoCharacter::RestoreCollisionAndMovement()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	bCanParkour = true;
}