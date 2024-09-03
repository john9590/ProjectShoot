// Fill out your copyright notice in the Description page of Project Settings.


#include "SHPlayer.h"
#include "SHBullets.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASHPlayer::ASHPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isRunning = false;
	isRotate = false;
	isZoom = false;
	BulletBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, TEXT("Bullets '/Game/Blueprints/Weapons/Bullets.Bullets_C'")));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0., 0., 90.));
	TpsCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPSCAMERA"));
	TpsCamera->SetupAttachment(SpringArm);
	FpsWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	FpsWeapon->SetupAttachment(TpsCamera);
}

// Called when the game starts or when spawned
void ASHPlayer::BeginPlay()
{
	Super::BeginPlay();
	//FActorSpawnParameters SpawnP;
	weapon = GetWorld()->SpawnActor<ASHWeapon>(FVector::ZeroVector,FRotator(90.,60.,-40.));
	if (weapon) {
		weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("b_RightHand"));
	}
	GetWorldTimerManager().SetTimer(FireTimer, this, &ASHPlayer::Fire, 0.1f, true);
	GetWorldTimerManager().PauseTimer(FireTimer);
	FpsWeapon->SetVisibility(false);
}

// Called every frame
void ASHPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!isRunning && GetCharacterMovement()->GetMaxSpeed() > 600.f) {
		//Tick 말고 TimeLine으로 바꿀생각
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
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASHPlayer::ZoomStart);
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
		//FpsWeapon->AddRelativeRotation(FRotator(0.f, 0.f, val));
	}
}

void ASHPlayer::Fire()
{
	FActorSpawnParameters param;
	if (isZoom) ASHBullets* bullet = GetWorld()->SpawnActor<ASHBullets>(BulletBP, 
		GetActorLocation() + GetActorUpVector() * 60.f, GetControlRotation(), param);
	else ASHBullets* bullet = GetWorld()->SpawnActor<ASHBullets>(BulletBP,
		GetActorLocation() + GetActorRightVector() * 75.f +
		GetActorUpVector() * 100., GetControlRotation(), param);
	//AddControllerPitchInput(-0.5f); //반동 제어 timeline 수정 필요
	//weapon->FireSet(GetActorLocation() + GetActorRightVector() * 75. + 
		//GetActorUpVector() * 100., GetControlRotation());//GetBaseAimRotation());
}

void ASHPlayer::FireStart()
{
	if (isRunning) StartStopRun();
	if (!isZoom) emitter = UGameplayStatics::SpawnEmitterAttached(MFAsset, weapon->GetRootComponent(), TEXT("MuzzleFlashSocket"));
	GetWorldTimerManager().UnPauseTimer(FireTimer);
}

void ASHPlayer::FireEnd()
{
	if (emitter) emitter->DestroyComponent();
	GetWorldTimerManager().PauseTimer(FireTimer);
}

void ASHPlayer::ZoomStart()
{
	//UE_LOG(LogTemp, Log, TEXT("asdfasdf"));
	isZoom = !isZoom;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, GetGameInstance()->GetFirstGamePlayer()->GetControllerId());
	if (PlayerController)
	{
		if (isZoom)
		{
			FireEnd();
			GetMesh()->SetVisibility(false);
			weapon->weapon->SetVisibility(false);
			FpsWeapon->SetVisibility(true);
			SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
			SpringArm->SocketOffset = FVector(0.f, 0.f, 0.f);
			SpringArm->TargetArmLength = 0.f;
		}
		else
		{
			FireEnd();
			GetMesh()->SetVisibility(true);
			weapon->weapon->SetVisibility(true);
			FpsWeapon->SetVisibility(false);
			SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
			SpringArm->SocketOffset = FVector(0.f, 75.f, 0.f);
			SpringArm->TargetArmLength = 300.f;
		}
	}
	//GetCapsuleMesh()
	//if (isZoom) FpsCamera->Activate();
	//else FpsCamera->Deactivate();
	//FVector Loc = weapon->weapon->GetBoneLocation(TEXT("MuzzleFlashSocket"), EBoneSpaces::ComponentSpace);
	//Loc += GetMesh()->GetBoneLocation(TEXT("b_RightHand"), EBoneSpaces::ComponentSpace);
	//Loc -= FVector(-300., -75., 0.);
	//SpringArm->SetRelativeLocation(Loc);
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
	if (isRunning) {
		GetCharacterMovement()->MaxWalkSpeed = 1200.f;
		if (isZoom) ZoomStart();
		FireEnd();
	}
}