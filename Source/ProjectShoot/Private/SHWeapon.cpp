// Fill out your copyright notice in the Description page of Project Settings.


#include "SHWeapon.h"

// Weapon의 SkeltalMesh를 설정해주는 초기화 함수.. Weapon에서 bullet을 발사하려해서 만들었지만 이젠 쓸 필요가 있을까.. ?
ASHWeapon::ASHWeapon()
{
	weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = weapon;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponAsset(TEXT("Rifle '/Game/Weapons/Rifle'"));
	if (WeaponAsset.Succeeded()) weapon->SkeletalMesh = WeaponAsset.Object;
}




