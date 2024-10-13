// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "SHBullets.generated.h"

UCLASS()
class ASHBullets : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHBullets();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USphereComponent* collision;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	UProjectileMovementComponent* MovementComp;
};
