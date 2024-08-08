// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "SHWeapon.h"
#include "SHPlayer.generated.h"

UCLASS()
class PROJECTSHOOT_API ASHPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASHPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveFront(float val);
	void MoveRight(float val);
	void StartStopRun();
	void LookRight(float val);
	void LookUp(float val);
	void Fire();
	void FireStart();
	void FireEnd();
	bool isRunning;
	bool isRotate;
	ASHWeapon* weapon;
	FTimerHandle FireTimer;
	UClass* BulletBP;
	UParticleSystemComponent* emitter;

public:
	UFUNCTION(BlueprintCallable)
	bool GetIsRunning();
	UFUNCTION(BlueprintCallable)
	bool GetIsRotate();
	UFUNCTION(BlueprintCallable)
	FRotator GetAim();
	UPROPERTY(EditAnyWhere)
	UParticleSystem* MFAsset;
};
