// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AxiosysDemoCharacter.generated.h"

class UAxiosysMovementComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A basic first person character
 */
UCLASS(abstract)
class AAxiosysDemoCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UAxiosysMovementComponent* AxiosysMovementComponent;

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* MouseLookAction;
	
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* CrouchAction;
	
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* ParkourAction;
	
public:
	AAxiosysDemoCharacter(const FObjectInitializer &ObjectInitializer);

protected:

	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();
	
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoCrouchStart();
	
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoCrouchEnd();
	
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoParkourStart();
	
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoParkourEnd();
	
	UPROPERTY(Category="Character Movement: Parkour", EditDefaultsOnly, BlueprintReadWrite)
	bool bCanParkour = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Movement: Parkour")
	TObjectPtr<UAnimMontage> VaultMontage; 
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Movement: Parkour")
	TObjectPtr<UAnimMontage> ClimbMontage; 
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Movement: Parkour")
	TObjectPtr<UAnimMontage> GettingUpMontage; 

protected:

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	

public:

	/** Returns the first person mesh **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION()
	void RestoreCollisionAndMovement();
};

