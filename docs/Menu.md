# 📋 Menu 클래스 문서

`UMenu`는 언리얼의 `UUserWidget`을 상속받은 클래스이며,  
Steam Subsystem 기반 멀티플레이 세션을 위한 UI 기능을 제공합니다.  
Host/Join 버튼 처리, 세션 연결 상태 핸들링, UI 전환 등을 포함하고 있습니다.

---

## 🧩 클래스 정의

```cpp
UCLASS()
class UMenu : public UUserWidget
```

- 블루프린트에서 UI 위젯으로 구성되며, 세션 처리를 위한 핵심 메뉴 클래스입니다.

---

## 🛠 주요 함수

### `void MenuSetup(int32 NumConnections, FString MatchType, FString LobbyPath)`

- 세션 생성 전 UI를 설정하고 화면에 표시  
- PlayerController에 UI 전용 입력 모드 설정  
- `SHSubsystem`의 Delegate와 연결됨

---

### `bool Initialize() override`

- 위젯 초기화 시 버튼 바인딩 등 초기 설정 수행

---

### `void NativeDestruct() override`

- UI가 제거될 때 호출되며, Delegate 해제 등 정리 처리

---

## 🌐 SHSubsystem Delegate 연동

```cpp
UFUNCTION()
void OnCreateSession(bool bWasSuccessful);
void OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
void OnDestroySession(bool bWasSuccessful);
```

- 세션 생성/검색/참가/삭제 결과를 받아 UI를 다음 상태로 전환함

---

## 🧠 버튼 처리

- `HostButton`, `JoinButton` 등의 UButton에 대해 클릭 시  
  `SHSubsystem`의 세션 함수를 호출

---

## ✅ 요약

- `UMenu`는 **세션 UI 처리를 담당하는 위젯 클래스**입니다.  
- `MenuSetup()`으로 실행되며 Viewport에 표시되고,  
- 세션 이벤트에 따른 콜백 함수들(`OnCreateSession`, `OnFindSession`, 등)을 통해 세션 연결 상태를 관리합니다.  
- Steam 기반 멀티플레이 진입을 위한 전처리 UI를 담당합니다.
