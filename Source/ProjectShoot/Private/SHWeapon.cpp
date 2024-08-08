// Fill out your copyright notice in the Description page of Project Settings.


#include "SHWeapon.h"

// Sets default values
ASHWeapon::ASHWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = weapon;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponAsset(TEXT("Rifle '/Game/Weapons/Rifle'"));
	if (WeaponAsset.Succeeded()) weapon->SkeletalMesh = WeaponAsset.Object;
}

// Called when the game starts or when spawned
void ASHWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASHWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



