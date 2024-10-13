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


public:	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USkeletalMeshComponent* weapon;

};
