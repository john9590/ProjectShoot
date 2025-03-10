# 🌐 SHSubsystem 클래스 문서

`USHSubsystem`은 `UGameInstanceSubsystem`을 상속하며,  
Steam 기반의 온라인 멀티플레이 세션을 관리합니다.  
세션 생성, 검색, 참가, 시작, 삭제 등을 처리하며,  
각 단계에 대한 Delegate 및 결과 브로드캐스트 기능을 제공합니다.

---

## 📡 주요 기능 요약

| 기능 | 설명 |
|------|------|
| 세션 생성 | Host를 통한 게임 세션 생성 |
| 세션 검색 | 서버 리스트 검색 후 Join 가능 |
| 세션 참가 | Join 버튼 클릭 시 자동 접속 |
| 세션 삭제 | 현재 세션 종료 |
| 세션 시작 | 게임 시작 후 상태 전환 |

---

## 🧩 주요 클래스 정의

```cpp
class USHSubsystem : public UGameInstanceSubsystem
```

- 온라인 기능을 전체 게임 인스턴스에서 사용할 수 있도록 설정

---

## 📶 Delegate 정의

```cpp
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionComplete, const TArray<FOnlineSessionSearchResult>&, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool);
```

- 각각의 세션 처리 단계에서 결과를 외부 UI 등과 연동할 수 있도록 Event 처리 가능

---

## 🛠 주요 함수

### `CreateSession()`

- 새로운 세션을 설정하고 Steam에 등록
- 설정 예시: PublicConnections = 4, IsLANMatch = false 등

### `FindSession()`

- 현재 존재하는 세션을 검색
- 검색된 세션 리스트를 `FMultiplayerOnFindSessionComplete`로 브로드캐스트

### `JoinSession()`

- 세션 ID로 해당 세션에 접속 시도
- 결과는 `FMultiplayerOnJoinSessionComplete`를 통해 전달

### `DestroySession()`

- 현재 세션을 종료하며 세션 초기화 처리

---

## 🎯 콜백 함수 예시

```cpp
void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
void OnFindSessionComplete(bool bWasSuccessful);
void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
```

- Steam 및 언리얼 세션 처리 결과를 받아 처리하는 내부용 함수

---

## ✅ 요약

- `USHSubsystem`은 Project Shoot의 **멀티플레이 기능의 핵심 컨트롤러**입니다.  
- Host/Join/Destroy 전 과정을 C++에서 제어하며, Blueprint에서도 손쉽게 연결할 수 있도록 Delegate 기반으로 설계되었습니다.  
- Subsystem 아키텍처를 활용하여 전체 인스턴스에서 안정적으로 관리할 수 있습니다.
