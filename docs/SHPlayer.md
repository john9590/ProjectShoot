# 🧍 SHPlayer 클래스 문서

`ASHPlayer`는 `ACharacter`를 상속하며,  
FPS/TPS 시점 전환, 총알 발사, 공격 처리, 피격 및 사망 처리, 줌, 이동, 달리기 등의  
플레이어 조작을 포함한 핵심 클래스입니다.

---

## 🎮 주요 기능 요약

| 기능 | 설명 |
|------|------|
| FPS / TPS 시점 전환 | Zoom 상태 여부에 따라 카메라 위치 및 Mesh 숨김 처리 |
| 총기 공격 | 일정 간격으로 총알 발사 (`FireTimer`) |
| Fire 이펙트 | 총기 입구에서 Particle 출력 |
| 피격 / 사망 처리 | 체력 감소, 0 이하일 경우 사망 |
| 이동 / 달리기 | 입력 기반 기본 이동 + 속도 조정 |
| 멀티플레이 대응 | Replication, 서버 피격 처리 구현 포함 |

---

## 🔧 주요 변수

```cpp
bool isZoom;
bool isFire;
bool isDeath;
float Health;
float bullettime;
UClass* BulletBP;

USpringArmComponent* SpringArm;
UCameraComponent* TPSCamera;
UParticleSystemComponent* muzzleEffect;
ASHWeapon* weapon;
```

- `isZoom`, `isFire`, `isDeath`: 상태 트래킹용 변수  
- `BulletBP`: 블루프린트로 작성된 총알 클래스  
- `SpringArm`, `TPSCamera`: 카메라 시점 전환용  
- `muzzleEffect`: 총구 화염 이펙트  
- `weapon`: 현재 무기 오브젝트

---

## 🧠 주요 함수

### `void callFire()`

- 줌 여부에 따라 총알 생성 위치 결정  
- `SpawnActor<ASHBullets>()`로 총알 발사  
- `bullettime` 간격으로 반복 호출됨 (0.1초)

---

### `void ZoomStart()`

- `isZoom` 상태 전환
- TPS ↔ FPS 카메라 위치 및 SpringArm 조정
- Mesh Visibility 조정

---

### `void FireStart() / FireEnd()`

- 총알 발사 시작 / 종료
- FireTimer 시작 및 muzzle 이펙트 Play/Stop 처리

---

### `float TakeDamage(...)`

```cpp
virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
```

- 피격 시 `Health` 감소
- 체력이 0 이하일 경우 `isDeath = true` 처리

---

## 🕹️ 입력 바인딩

- `SetupPlayerInputComponent()`에서  
  - Fire / Zoom / Run / Move / Look 관련 입력 바인딩

---

## ✅ 요약

- `ASHPlayer`는 **TPS/FPS 전환 슈팅 플레이어 캐릭터의 핵심 로직**을 담당합니다.
- 공격, 피격, 상태 변화, 입력 처리까지 모두 관리하며 멀티플레이 환경에서도 정상 동작하도록 설계되어 있습니다.
- SpringArm, Particle, FireTimer, Replication 등 언리얼의 다양한 기능이 집약된 클래스입니다.
