// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SHBullets.generated.h"

UCLASS()
class ASHBullets : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHBullets();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USphereComponent* collision;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	UProjectileMovementComponent* MovementComp;

};
