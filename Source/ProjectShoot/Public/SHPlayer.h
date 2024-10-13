// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
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
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void MoveFront(float val);
	void MoveRight(float val);
	void StartStopRun();
	void LookRight(float val);
	void LookUp(float val);
	void FireStart();
	void FireEnd();
	void ZoomStart();
	void CondJump();

	// TPS상태에서 유저가 들고있는 weapon을 나타냄
	ASHWeapon* weapon;
	FTimerHandle FireTimer;
	// SHBullet을 부모로 하는 블루프린트를 불러옴, 이를 생성하여 총알을 발사함
	UClass* BulletBP;
	UParticleSystemComponent* emitter;
	float bullettime;

public:
	UFUNCTION(BlueprintCallable)
	bool GetIsRunning();
	UFUNCTION(BlueprintCallable)
	bool GetIsRotate();
	UFUNCTION(BlueprintCallable)
	bool GetIsDeath();
	UFUNCTION(BlueprintCallable)
	FRotator GetAim();
	UPROPERTY(EditAnyWhere)
	UParticleSystem* MFAsset;
	// Camera의 위치를 플레이에 상대적으로 바꾸기 위해 USpringArmComponent를 이용하여 만든 변수, 이를 조절하여 시점을 변경함
	USpringArmComponent* SpringArm;
	// 이 위치가 유저의 시점이 되어 플레이어를 볼 수 있게 함
	UCameraComponent* TpsCamera;
	// FPS로 시점이 변경되었을 때 유저 앞에 조준선과 조준기가 보이기 위함
	USkeletalMeshComponent* FpsWeapon;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Health)
	float Health;

	UFUNCTION(Server, Reliable, WithValidation)
	void serverHealth(float delta);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BPHealth(AActor* delta);

	void callFire();

	UFUNCTION(BlueprintImplementableEvent)
	void Fire();
	UFUNCTION(Server, Reliable, WithValidation)
	void serverzoom();
	UFUNCTION(Server, Reliable, WithValidation)
	void serverFire();
	UFUNCTION(Server, Reliable, WithValidation)
	void serverRun();
	UFUNCTION(Server, Reliable, WithValidation)
	void serverModifySpeed(float speed);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = run)
	bool isRunning;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = rotate)
	bool isRotate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = zoom)
	bool isZoom;
	bool isFire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = death)
	bool isDeath;
};
