// Fill out your copyright notice in the Description page of Project Settings.


#include "SHPlayer.h"
#include "SHBullets.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASHPlayer::ASHPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isRunning = false;
	isRotate = false;
	BulletBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, TEXT("Bullets '/Game/Blueprints/Weapons/Bullets.Bullets_C'")));
}

// Called when the game starts or when spawned
void ASHPlayer::BeginPlay()
{
	Super::BeginPlay();
	FName Socket(TEXT("b_RightHand"));
	//FActorSpawnParameters SpawnP;
	weapon = GetWorld()->SpawnActor<ASHWeapon>(FVector::ZeroVector,FRotator(90.,60.,-40.));
	if (weapon) {
		weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Socket);
	}
	GetWorldTimerManager().SetTimer(FireTimer, this, &ASHPlayer::Fire, 0.1f, true);
	GetWorldTimerManager().PauseTimer(FireTimer);
}

// Called every frame
void ASHPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!isRunning && GetCharacterMovement()->GetMaxSpeed() > 600.f) {
		GetCharacterMovement()->MaxWalkSpeed = fmaxf(GetCharacterMovement()->MaxWalkSpeed - 2048.f * DeltaTime, 600.f);
	}
}

// Called to bind functionality to input
void ASHPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveFront", this, &ASHPlayer::MoveFront);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASHPlayer::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASHPlayer::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ASHPlayer::LookRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASHPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASHPlayer::StopJumping);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASHPlayer::StartStopRun);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASHPlayer::FireStart);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASHPlayer::FireEnd);
}

void ASHPlayer::MoveFront(float val)
{
	if (val != 0.f && Controller) {
		AddMovementInput(GetActorForwardVector(), val);
	}
	else if (isRunning) StartStopRun();
}

void ASHPlayer::MoveRight(float val)
{
	if (val != 0.f && Controller) {
		if (isRunning) StartStopRun();
		AddMovementInput(GetActorRightVector(), val);
	}
}

void ASHPlayer::LookRight(float val)
{
	if (val != 0.f && Controller) {
		AddControllerYawInput(val);
	}
	if (val == 0.f) isRotate = false;
	else isRotate = true;
}

void ASHPlayer::LookUp(float val)
{
	if (val != 0.f && Controller) {
		AddControllerPitchInput(val);
	}
}

void ASHPlayer::Fire()
{
	FActorSpawnParameters param;
	ASHBullets* bullet = GetWorld()->SpawnActor<ASHBullets>(BulletBP,
		GetActorLocation() + GetActorRightVector() * 75. +
		GetActorUpVector() * 100., GetControlRotation(), param);
	//AddControllerPitchInput(-0.5f); //�ݵ� ���� timeline ���� �ʿ�
	//weapon->FireSet(GetActorLocation() + GetActorRightVector() * 75. + 
		//GetActorUpVector() * 100., GetControlRotation());//GetBaseAimRotation());
}

void ASHPlayer::FireStart()
{
	emitter = UGameplayStatics::SpawnEmitterAttached(MFAsset, weapon->GetRootComponent(), TEXT("MuzzleFlashSocket"));
	GetWorldTimerManager().UnPauseTimer(FireTimer);
}

void ASHPlayer::FireEnd()
{
	if (emitter) emitter->DestroyComponent();
	GetWorldTimerManager().PauseTimer(FireTimer);
}

bool ASHPlayer::GetIsRunning()
{
	return isRunning;
}

bool ASHPlayer::GetIsRotate()
{
	return isRotate;
}

FRotator ASHPlayer::GetAim()
{
	return ActorToWorld().InverseTransformVector(
		GetBaseAimRotation().Vector()).Rotation();
}

void ASHPlayer::StartStopRun()
{
	isRunning = !isRunning;
	if (isRunning) GetCharacterMovement()->MaxWalkSpeed = 1200.f;
}
