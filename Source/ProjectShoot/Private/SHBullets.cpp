// Fill out your copyright notice in the Description page of Project Settings.


#include "SHBullets.h"
#include "SHWeapon.h"
#include "Kismet/GameplayStatics.h"

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

void ASHBullets::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Log, TEXT("%s == %s"), *(GetActorLocation().ToString()), *(GetInstigator()->GetName()));
	if (!GetInstigator()) return;
	if (OtherActor == GetInstigator()) return;
	if (Cast<ASHBullets>(OtherActor)) return;
	if (Cast<ASHWeapon>(OtherActor)) return;
	if (Cast<ACharacter>(OtherActor)) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BDAsset, GetActorTransform(), true);
	else UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), IHAsset, GetActorTransform(), true);
	UE_LOG(LogTemp, Log, TEXT("%s == %s"), *(OtherActor->GetName()), *(GetInstigator()->GetName()));
	//UGameplayStatics::ApplyDamage(OtherActor, 10.f, GetInstigator()->GetController(), this, UDamageType::StaticClass());
	while (!Destroy());
}

void ASHBullets::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("%s == %s"), *(GetActorLocation().ToString()), *(GetInstigator()->GetName()));
	if (OtherActor == GetInstigator()) return;
	if (Cast<ASHWeapon>(OtherActor)) return;
	if (Cast<ACharacter>(OtherActor)) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BDAsset, GetActorTransform(), true);
	else UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), IHAsset, GetActorTransform(), true);
	UE_LOG(LogTemp, Log, TEXT("%s == %s"), *(OtherActor->GetName()), *(GetInstigator()->GetName()));
	UGameplayStatics::ApplyDamage(OtherActor, 10.f, GetInstigator()->GetController(), this, UDamageType::StaticClass());
	while (!Destroy());
}

