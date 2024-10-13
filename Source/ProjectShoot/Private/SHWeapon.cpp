// Fill out your copyright notice in the Description page of Project Settings.


#include "SHWeapon.h"

// Weapon�� SkeltalMesh�� �������ִ� �ʱ�ȭ �Լ�.. Weapon���� bullet�� �߻��Ϸ��ؼ� ��������� ���� �� �ʿ䰡 ������.. ?
ASHWeapon::ASHWeapon()
{
	weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = weapon;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponAsset(TEXT("Rifle '/Game/Weapons/Rifle'"));
	if (WeaponAsset.Succeeded()) weapon->SkeletalMesh = WeaponAsset.Object;
}




