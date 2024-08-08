// Fill out your copyright notice in the Description page of Project Settings.


#include "SHEffect.h"

// Sets default values
ASHEffect::ASHEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PARTICLE"));
	RootComponent = ParticleComp;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MFAssetF(TEXT("P_AssaultRifle_MF '/Game/Effects/ParticleSystems/Weapons/AssaultRifle/Muzzle/P_AssaultRifle_MF'"));
	if (MFAssetF.Succeeded()) MFAsset = MFAssetF.Object;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> IHAssetF(TEXT("P_AssaultRifle_IH '/Game/Effects/ParticleSystems/Weapons/AssaultRifle/Impacts/P_AssaultRifle_IH'"));
	if (IHAssetF.Succeeded()) IHAsset = IHAssetF.Object;
	ParticleComp->SetTemplate(MFAsset);
}

// Called when the game starts or when spawned
void ASHEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASHEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

