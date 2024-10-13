// Fill out your copyright notice in the Description page of Project Settings.


#include "SHPlayer.h"
#include "SHBullets.h"
#include "SHPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// 기본적으로 변수들을 초기화 하고 SpringArm, TPSCamera 등에 SubObject을 생성하여 대입하고 SubObject들의 위치와 회전값을 조정하여 플레이어를 생성
ASHPlayer::ASHPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	isRunning = false;
	isRotate = false;
	isZoom = false;
	isFire = false;
	isDeath = false;
	Health = 100.f;
	bullettime = 0.f;

	BulletBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, TEXT("Bullets '/Game/Blueprints/Weapons/Bullets.Bullets_C'")));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0., 0., 90.));
	SpringArm->SocketOffset = FVector(0.f, 75.f, 0.f);
	SpringArm->bUsePawnControlRotation = true;
	TpsCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPSCAMERA"));
	TpsCamera->SetupAttachment(SpringArm);
	FpsWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	FpsWeapon->SetupAttachment(TpsCamera);
	FpsWeapon->SetRelativeLocation(FVector(0.f, 0.f, -20.f));
	FpsWeapon->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponAsset(TEXT("Rifle '/Game/Weapons/Rifle'"));
	if (WeaponAsset.Succeeded()) FpsWeapon->SkeletalMesh = WeaponAsset.Object;
	GetCapsuleComponent()->SetCapsuleHalfHeight(110.f);
	GetCapsuleComponent()->SetCapsuleRadius(65.f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("/Game/Blueprints/Animations/TPP_Anim"));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -86.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -75.f, 0.f));
	if (AnimBP.Succeeded()) GetMesh()->SetAnimClass(AnimBP.Class);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("HeroTPP '/Game/Characters/HeroTPP/HeroTPP.HeroTPP'"));
	if (MeshAsset.Succeeded()) GetMesh()->SkeletalMesh = MeshAsset.Object;
	GetCharacterMovement()->JumpZVelocity = 840.f;
}

// TPS 시점의 weapon을 Mesh의 b_Righthand Bone에 Attach하고 0.1초에 한번씩 실행되는 Timer를 설정
void ASHPlayer::BeginPlay()
{
	Super::BeginPlay();
	weapon = GetWorld()->SpawnActor<ASHWeapon>(FVector::ZeroVector,FRotator(90.,60.,-40.));
	if (weapon) {
		weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("b_RightHand"));
	}
	GetWorldTimerManager().SetTimer(FireTimer, this, &ASHPlayer::callFire, 0.1f, true);
	GetWorldTimerManager().PauseTimer(FireTimer);
	FpsWeapon->SetVisibility(false);
}

// 달리는 상태에서 걷는 상태로 돌아올 때 MaxSpeed를 천천히 줄어주는 기능
void ASHPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!isRunning && GetCharacterMovement()->GetMaxSpeed() > 600.f) {
		//Tick 말고 TimeLine으로 바꿀생각
		if (!HasAuthority()) serverModifySpeed(fmaxf(GetCharacterMovement()->MaxWalkSpeed - 2048.f * DeltaTime, 600.f));
		GetCharacterMovement()->MaxWalkSpeed = fmaxf(GetCharacterMovement()->MaxWalkSpeed - 2048.f * DeltaTime, 600.f);
	}
}

// 들어오는 입력을 함수와 바인딩해서 실제로 기능이 작동하도록 만들어주는 함수
void ASHPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveFront", this, &ASHPlayer::MoveFront);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASHPlayer::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASHPlayer::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ASHPlayer::LookRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASHPlayer::CondJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASHPlayer::StopJumping);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASHPlayer::StartStopRun);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASHPlayer::FireStart);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASHPlayer::FireEnd);
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASHPlayer::ZoomStart);
}

// Bullet을 통해 맞았을 때 Bullet에서 ApplyDamage를 통해 실행되는 함수, 맞은 데미지 만큼 플레이어의 Health를 깎고 Health가 0이하면 Death처리를 한다.
float ASHPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (DamageAmount > 0.0f) {
		if (!HasAuthority()) serverHealth(DamageAmount);
		else {
			Health -= DamageAmount;
			if (Health <= 0.f) {
				Health = 0.f;
				isDeath = true;
				GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
	return ActualDamage;
}

void ASHPlayer::MoveFront(float val)
{
	if (isDeath) return;
	if (val != 0.f && Controller) {
		AddMovementInput(GetActorForwardVector(), val);
	}
	else if (isRunning) StartStopRun();
}

void ASHPlayer::MoveRight(float val)
{
	if (isDeath) return;
	if (val != 0.f && Controller) {
		if (isRunning) StartStopRun();
		AddMovementInput(GetActorRightVector(), val);
	}
}

void ASHPlayer::LookRight(float val)
{
	if (isDeath) return;
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


void ASHPlayer::serverModifySpeed_Implementation(float speed) {
	GetCharacterMovement()->MaxWalkSpeed = speed;
}

bool ASHPlayer::serverModifySpeed_Validate(float speed) {
	return true;
}

void ASHPlayer::serverFire_Implementation() {
	ASHBullets* bullet;
	FActorSpawnParameters param;
	FVector location = GetActorLocation();
	if (isZoom) location += GetActorUpVector() * 60.f;
	else location += GetActorRightVector() * 75.f + GetActorUpVector() * 100.f;
	bullet = GetWorld()->SpawnActor<ASHBullets>(BulletBP, location, GetControlRotation(), param);
	if (bullet) {
		bullet->SetInstigator(this);
		bullet->collision->IgnoreActorWhenMoving(this, true);
	}
	//AddControllerPitchInput(-0.5f); //반동 제어 timeline 수정 필요
}

bool ASHPlayer::serverFire_Validate() {
	return true;
}

void ASHPlayer::callFire()
{
	if (!HasAuthority()) serverFire();
	else {
		ASHBullets* bullet;
		FActorSpawnParameters param;
		FVector location = GetActorLocation();
		if (isZoom) location += GetActorUpVector() * 60.f;
		else location += GetActorRightVector() * 75.f + GetActorUpVector() * 100.f;
		bullet = GetWorld()->SpawnActor<ASHBullets>(BulletBP, location, GetControlRotation(), param);
		if (bullet) {
			bullet->SetInstigator(this);
			bullet->collision->IgnoreActorWhenMoving(this, true);
		}
	}
}


// LeftClick Pressed시에 실행되는 함수로 총구에 화염 효과를 넣어주며 FireTimer를 시작함
void ASHPlayer::FireStart()
{
	if (isDeath) return;
	if (isRunning) StartStopRun();
	if (!isZoom) emitter = UGameplayStatics::SpawnEmitterAttached(MFAsset, weapon->GetRootComponent(), TEXT("MuzzleFlashSocket"));
	isFire = true;
	bullettime = 0.f;
	GetWorldTimerManager().UnPauseTimer(FireTimer);
}

// LeftClick Released시에 실행되는 함수로 총구에 화염 효과를 끝내고 FireTimer를 중단함
void ASHPlayer::FireEnd()
{
	if (isDeath) return;
	if (emitter) emitter->Complete();
	GetWorldTimerManager().PauseTimer(FireTimer);
	emitter = NULL;
	isFire = false;
}

void ASHPlayer::ZoomStart()
{
	if (isDeath) return;
	if (!HasAuthority()) {
		serverzoom();
	}
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

}

void ASHPlayer::CondJump()
{
	if (isDeath) return;
	Jump();
}

void ASHPlayer::serverzoom_Implementation() {
	isZoom = !isZoom;
}

bool ASHPlayer::serverzoom_Validate() {
	return true;
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

bool ASHPlayer::GetIsDeath()
{
	return isDeath;
}

void ASHPlayer::serverRun_Implementation() {
	isRunning = !isRunning;
	if (isRunning) {
		GetCharacterMovement()->MaxWalkSpeed = 1200.f;
		if (isZoom) ZoomStart();
		FireEnd();
	}
}

bool ASHPlayer::serverRun_Validate() {
	return true;
}

void ASHPlayer::StartStopRun()
{
	if (isDeath) return;
	if (!HasAuthority()) serverRun();
	isRunning = !isRunning;
	if (isRunning) {
		GetCharacterMovement()->MaxWalkSpeed = 1200.f;
		if (isZoom) ZoomStart();
		FireEnd();
	}
}

void ASHPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASHPlayer, Health);
	DOREPLIFETIME(ASHPlayer, isZoom);
	DOREPLIFETIME(ASHPlayer, isDeath);
	DOREPLIFETIME(ASHPlayer, isRotate);
	DOREPLIFETIME(ASHPlayer, isRunning);
}

void ASHPlayer::MultiUpdateHealth_Implementation(float NewHealth)
{
	// 클라이언트에서 호출될 때 새로운 체력 값으로 업데이트
	Health = NewHealth;
	UE_LOG(LogTemp, Log, TEXT("Health updated to: %f"), NewHealth);
}

void ASHPlayer::serverHealth_Implementation(float delta)
{
	Health -= delta;
	if (Health <= 0.f) {
		Health = 0.f;
		isDeath = true;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

bool ASHPlayer::serverHealth_Validate(float delta)
{
	return delta >= 0.f;
}
