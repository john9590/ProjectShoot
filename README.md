# 🎯 Project Shoot - TPS/FPS 기반 멀티플레이 슈팅 게임

> 언리얼 엔진 5 기반으로 개발된 1인 슈팅 프로젝트입니다.  
> TPS ↔ FPS 시점 전환, 총격 처리, 데미지 및 이펙트,  
> Steam 기반 멀티플레이 시스템까지 전반적인 전투 시스템을 구현했습니다.

---

## 🧩 핵심 기능 요약

| 시스템 | 설명 |
|--------|------|
| TPS/FPS 시점 전환 | RightClick으로 시점 변경 (SpringArm, Camera, Mesh Toggle) |
| 총격 / 피격 / 사망 | FireTimer 기반 총알 발사 + 체력 시스템 |
| 멀티플레이 세션 | Steam Subsystem 기반 Host/Join 처리 |
| UI 시스템 | Lobby Menu + 게임 중 UI 제어 |
| 네트워크 복제 | 총알, 상태, 세션 등 주요 정보 Replicated 처리 |
| TPS 애니메이션 | 상태 전이에 따라 자연스럽게 동작 (TPP_ANIM) |

---

## 🎮 조작 키

| 동작 | 키 |
|------|----|
| 이동 | WASD |
| 점프 | Space |
| 달리기 | Left Shift |
| 공격 | Left Click |
| 시점 전환 (Zoom) | Right Click |
| 마우스 회전 | 마우스 이동 |

---

## 🔧 주요 클래스 & 문서

| 클래스 | 역할 | 문서 |
|--------|------|------|
| `SHPlayer` | 시점 전환 + 공격/이펙트 처리 | [📄 SHPlayer.md](docs/SHPlayer.md) |
| `SHWeapon` | 캐릭터가 사용하는 무기 액터 | [📄 SHWeapon.md](docs/SHWeapon.md) |
| `SHBullets` | 총알 투사체 + 충돌 처리 이펙트 | [📄 SHBullets.md](docs/SHBullets.md) |
| `TPP_ANIM` | TPS 애니메이션 블렌딩 로직 | [📄 TPP_ANIM.md](docs/TPP_ANIM.md) |
| `SHSubsystem` | 세션 생성/검색/참가 처리 | [📄 SHSubsystem.md](docs/SHSubsystem.md) |
| `SHGamemode` | GameMode에서 세션 처리 | [📄 SHGamemode.md](docs/SHGamemode.md) |
| `Menu` | UMG 위젯 기반 멀티플레이 메뉴 UI | [📄 Menu.md](docs/Menu.md) |

---

## 🎥 시연 영상

[![Watch Demo](http://img.youtube.com/vi/Z8kIrM9yURE/0.jpg)](https://www.youtube.com/watch?v=Z8kIrM9yURE)

---

## 💡 프로젝트 회고

- 처음으로 Steam 기반의 멀티플레이 구현을 직접 경험하며,
  Subsystem 구조와 세션 흐름을 깊이 이해할 수 있었습니다.
- TPS ↔ FPS 시점 전환 시 발생하는 조준점 차이를 직접 해결하며,
  3D 게임 내에서 시점과 조작의 관계에 대한 감각을 키웠습니다.
- 애니메이션 상태 전이, 충돌 이펙트 등 시각적 피드백 요소까지 고려한 설계 경험이 인상 깊었습니다.

---

## 🔗 링크

- 📂 [GitHub Repository](https://github.com/john9590/ProjectShoot)
- 🎞️ [시연 영상](https://youtu.be/Z8kIrM9yURE)
