// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "SHBullets.generated.h"

UCLASS()
class ASHBullets : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHBullets();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USphereComponent* collision;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	UProjectileMovementComponent* MovementComp;
	UPROPERTY(EditAnyWhere)
	UParticleSystem* IHAsset;
	UPROPERTY(EditAnyWhere)
	UParticleSystem* BDAsset;
	TWeakObjectPtr<AController> MyController;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerApplyDamage(AActor* DamagedActor, float DamageAmount);

	void ServerApplyDamage_Implementation(AActor* DamagedActor, float DamageAmount);

	bool ServerApplyDamage_Validate(AActor* DamagedActor, float DamageAmount)
	{
		return true; 
	}

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerApply(const FHitResult& HitResult);
	void ServerApply_Implementation(const FHitResult& HitResult);
	bool ServerApply_Validate(const FHitResult& HitResult) {
		return true;
	}

private:
};
