// Fill out your copyright notice in the Description page of Project Settings.


#include "SHBullets.h"
#include "SHWeapon.h"
#include "SHPlayer.h"
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
	bReplicates = true;
	if (HasAuthority())
		SetOwner(GetInstigatorController());
}

void ASHBullets::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MovementComp->OnProjectileStop.AddDynamic(this, &ASHBullets::ServerApply);
	collision->MoveIgnoreActors.Add(GetInstigator());

	MyController = GetInstigatorController();
}

// Called when the game starts or when spawned
void ASHBullets::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("Server owns this bullet"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Client owns this bullet"));
	}

	if (GetOwner()) {
		UE_LOG(LogTemp, Warning, TEXT("Bullet owner: %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void ASHBullets::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASHBullets::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Log, TEXT("%s == %s"), *(GetActorLocation().ToString()), *(GetInstigator()->GetName()));
	/*if (!GetInstigator()) return;
	if (OtherActor == GetInstigator()) return;
	if (Cast<ASHBullets>(OtherActor)) return;
	if (Cast<ASHWeapon>(OtherActor)) return;
	if (ASHPlayer* temp = Cast<ASHPlayer>(OtherActor)) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BDAsset, GetActorTransform(), true);
		temp->BPHealth(10.f);
		//else ServerApplyDamage(OtherActor, 10.f, GetInstigator()->GetController(), this);
	}
	else UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), IHAsset, GetActorTransform(), true);
	UE_LOG(LogTemp, Log, TEXT("%s == %s"), *(OtherActor->GetName()), *(GetInstigator()->GetName()));
	while (!Destroy());*/
}

void ASHBullets::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	/*UE_LOG(LogTemp, Log, TEXT("Tqtjfak"));
	if (OtherActor == GetInstigator()) return;
	if (Cast<ASHWeapon>(OtherActor)) return;
	if (ASHPlayer* temp = Cast<ASHPlayer>(OtherActor)) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BDAsset, GetActorTransform(), true);
		if (!HasAuthority())
			temp->serverHealth(10.f);
		else
			ServerApplyDamage(temp, 10.f);
	}
	else UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), IHAsset, GetActorTransform(), true);
	UE_LOG(LogTemp, Log, TEXT("%s == %s"), *(OtherActor->GetName()), *(GetInstigator()->GetName()));
	//UGameplayStatics::ApplyDamage(OtherActor, 10.f, GetInstigator()->GetController(), this, UDamageType::StaticClass());
	while (!Destroy());*/
}

void ASHBullets::ServerApply_Implementation(const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("dlrpehlsp"));
}

void ASHBullets::ServerApplyDamage_Implementation(AActor* DamagedActor, float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerApplyDamage_Implementation called"));
	if (DamagedActor) {
		ASHPlayer* temp = Cast<ASHPlayer>(DamagedActor);
		if (temp) {
			temp->serverHealth(DamageAmount);
		}
	}
}
