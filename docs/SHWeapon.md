# 🔫 SHWeapon 클래스 문서

`ASHWeapon`은 플레이어 또는 캐릭터가 장착하는 무기를 나타내는 `AActor` 기반 클래스입니다.  
무기 모델의 시각적 표현을 담당하며, 무기의 메쉬 구성 및 RootComponent 설정이 중심입니다.

---

## 🧩 클래스 정의

```cpp
UCLASS()
class ASHWeapon : public AActor
```

- 무기를 별도 액터로 분리하여 소켓에 부착하거나, 시각적 구성에 활용

---

## 🔧 주요 변수

```cpp
UPROPERTY(EditAnyWhere, BlueprintReadWrite)
USkeletalMeshComponent* weapon;
```

- `weapon`: SkeletalMesh 기반의 무기 모델  
  (예: 라이플, 샷건 등 3D 무기 메시 구성)

---

## 🛠 생성자

```cpp
ASHWeapon::ASHWeapon() {
    weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
    RootComponent = weapon;

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponAsset(TEXT("Rifle '/Game/Weapons/Rifle'"));
    if (WeaponAsset.Succeeded())
        weapon->SkeletalMesh = WeaponAsset.Object;
}
```

- 무기 메쉬 컴포넌트를 생성하고 루트 컴포넌트로 설정  
- `ConstructorHelpers::FObjectFinder`를 통해 무기 에셋을 불러옴

---

## 💡 특징

- 이 무기 액터는 보통 플레이어 캐릭터의 `b_RightHand` 소켓 등에 부착되어 사용됩니다.
- 무기 자체에서 총알을 발사하지 않으며, 발사는 `SHPlayer`에서 처리됩니다.
- 시각적 무기만 독립시켜 구성하면 다양한 무기 스왑이 용이해집니다.

---

## ✅ 요약

- `ASHWeapon`은 무기의 **시각적 구성 및 부착을 담당하는 컴포넌트 중심 액터**입니다.  
- Unreal의 SkeletalMeshComponent를 활용하여 3D 무기 모델을 구성하고,  
  다른 클래스(`SHPlayer`)에서 실제 발사 로직을 연동합니다.
