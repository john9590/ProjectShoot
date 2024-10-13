// Fill out your copyright notice in the Description page of Project Settings.


#include "SHPlayer.h"
#include "SHBullets.h"
#include "SHPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// �⺻������ �������� �ʱ�ȭ �ϰ� SpringArm, TPSCamera � SubObject�� �����Ͽ� �����ϰ� SubObject���� ��ġ�� ȸ������ �����Ͽ� �÷��̾ ����
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

// TPS ������ weapon�� Mesh�� b_Righthand Bone�� Attach�ϰ� 0.1�ʿ� �ѹ��� ����Ǵ� Timer�� ����
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

// �޸��� ���¿��� �ȴ� ���·� ���ƿ� �� MaxSpeed�� õõ�� �پ��ִ� ���
void ASHPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!isRunning && GetCharacterMovement()->GetMaxSpeed() > 600.f) {
		//Tick ���� TimeLine���� �ٲܻ���
		if (!HasAuthority()) serverModifySpeed(fmaxf(GetCharacterMovement()->MaxWalkSpeed - 2048.f * DeltaTime, 600.f));
		GetCharacterMovement()->MaxWalkSpeed = fmaxf(GetCharacterMovement()->MaxWalkSpeed - 2048.f * DeltaTime, 600.f);
	}
}

// ������ �Է��� �Լ��� ���ε��ؼ� ������ ����� �۵��ϵ��� ������ִ� �Լ�
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

// Bullet�� ���� �¾��� �� Bullet���� ApplyDamage�� ���� ����Ǵ� �Լ�, ���� ������ ��ŭ �÷��̾��� Health�� ��� Health�� 0���ϸ� Deathó���� �Ѵ�.
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
	//AddControllerPitchInput(-0.5f); //�ݵ� ���� timeline ���� �ʿ�
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


// LeftClick Pressed�ÿ� ����Ǵ� �Լ��� �ѱ��� ȭ�� ȿ���� �־��ָ� FireTimer�� ������
void ASHPlayer::FireStart()
{
	if (isDeath) return;
	if (isRunning) StartStopRun();
	if (!isZoom) emitter = UGameplayStatics::SpawnEmitterAttached(MFAsset, weapon->GetRootComponent(), TEXT("MuzzleFlashSocket"));
	isFire = true;
	bullettime = 0.f;
	GetWorldTimerManager().UnPauseTimer(FireTimer);
}

// LeftClick Released�ÿ� ����Ǵ� �Լ��� �ѱ��� ȭ�� ȿ���� ������ FireTimer�� �ߴ���
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
	// Ŭ���̾�Ʈ���� ȣ��� �� ���ο� ü�� ������ ������Ʈ
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
