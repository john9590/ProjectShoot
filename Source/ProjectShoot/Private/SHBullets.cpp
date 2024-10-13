// Fill out your copyright notice in the Description page of Project Settings.


#include "SHBullets.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASHBullets::ASHBullets()
{
	collision = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE"));
	collision->SetupAttachment(RootComponent);
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("COMPONENT"));
	bReplicates = true;
}

