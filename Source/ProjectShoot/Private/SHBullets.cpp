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
	collision->OnComponentBeginOverlap.AddDynamic(this, &ASHBullets::BeginOverlap);
	collision->OnComponentHit.AddDynamic(this, &ASHBullets::OnHit);
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

void ASHBullets::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Other->GetActorLocation();
	while(!Destroy());
}

void ASHBullets::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
}

