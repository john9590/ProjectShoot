// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "SHWeapon.generated.h"

UCLASS()
class ASHWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Fire(FVector pos, FRotator rot);
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USkeletalMeshComponent* weapon;

private:
	UClass* BulletBP;
};
