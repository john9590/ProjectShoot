// Fill out your copyright notice in the Description page of Project Settings.


#include "SHPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASHPlayer::ASHPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isRunning = false;
}

// Called when the game starts or when spawned
void ASHPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASHPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASHPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveFront", this, &ASHPlayer::MoveFront);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASHPlayer::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASHPlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &ASHPlayer::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASHPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASHPlayer::StopJumping);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASHPlayer::StartStopRun);
}

void ASHPlayer::MoveFront(float val)
{
	if (val != 0.0f && Controller) {
		AddMovementInput(GetActorForwardVector(), val);
	}
}

void ASHPlayer::MoveRight(float val)
{
	if (val != 0.0f && Controller) {
		AddMovementInput(GetActorRightVector(), val);
	}
}

//void ASHPlayer::StartStopRun()
//{
//	isRunning = !isRunning;
//	if (isRunning) GetCharacterMovement()->MaxWalkSpeed = 1200.f;
//	else GetCharacterMovement()->MaxWalkSpeed = 600.f;
//}
