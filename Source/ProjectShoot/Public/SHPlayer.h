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

	// TPS���¿��� ������ ����ִ� weapon�� ��Ÿ��
	ASHWeapon* weapon;
	FTimerHandle FireTimer;
	// SHBullet�� �θ�� �ϴ� �������Ʈ�� �ҷ���, �̸� �����Ͽ� �Ѿ��� �߻���
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
	// Camera�� ��ġ�� �÷��̿� ��������� �ٲٱ� ���� USpringArmComponent�� �̿��Ͽ� ���� ����, �̸� �����Ͽ� ������ ������
	USpringArmComponent* SpringArm;
	// �� ��ġ�� ������ ������ �Ǿ� �÷��̾ �� �� �ְ� ��
	UCameraComponent* TpsCamera;
	// FPS�� ������ ����Ǿ��� �� ���� �տ� ���ؼ��� ���رⰡ ���̱� ����
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
