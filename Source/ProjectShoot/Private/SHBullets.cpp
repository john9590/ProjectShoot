// Fill out your copyright notice in the Description page of Project Settings.


#include "SHBullets.h"

// Sets default values
ASHBullets::ASHBullets()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = true;
	collision = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE"));
	collision->SetupAttachment(RootComponent);
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("COMPONENT"));
}

// Called when the game starts or when spawned
void ASHBullets::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASHBullets::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

