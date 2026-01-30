// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AxiosysMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXIOSYSDEMO_API UAxiosysMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAxiosysMovementComponent();
	
	UPROPERTY(Category="Character Movement: Parkour", EditDefaultsOnly, BlueprintReadWrite)
	bool bCanParkour;
	
	//UFUNCTION(Category="Character Movement", BlueprintCallable)
	//void StartParkour();
};
