# 🎮 SHGamemode 클래스 문서

`ASHGamemode`는 언리얼의 `AGameMode`를 상속하여  
게임 시작 시 Steam 세션 처리를 담당하는 GameMode 클래스입니다.  
세션 생성, 검색, 참가 기능을 직접 처리하며 Debug 메시지를 통해 상태 확인도 가능합니다.

---

## 🧩 클래스 정의

```cpp
UCLASS()
class ASHGamemode : public AGameMode
```

- 세션 처리 로직이 내장된 멀티플레이 GameMode  
- Subsystem 사용 없이 직접 OnlineSessionInterface를 통해 세션 처리

---

## 🔧 주요 변수

```cpp
TSharedPtr<FOnlineSessionSearch> SessionSearch;
IOnlineSessionPtr OnlineSessionInterface;

FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
FOnFindSessionsCompleteDelegate FindSessionCompleteDelegate;
FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
```

- `OnlineSessionInterface`: 세션 처리의 중심 인터페이스  
- 각 Delegate는 세션 처리 완료 시 콜백 연결

---

## 🛠 주요 함수

### `ASHGamemode::ASHGamemode()`

- Subsystem을 직접 가져와서 `OnlineSessionInterface` 초기화  
- Delegate 연결  
- `GEngine->AddOnScreenDebugMessage`로 디버깅 메시지 출력

---

### `void CreateGameSession()`

- 세션을 설정하고 생성 요청을 보냄

### `void JoinGameSession()`

- 검색된 세션에 접속을 시도함

---

### Delegate 콜백 함수

```cpp
void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
void OnFindSessionComplete(bool bWasSuccessful);
void OnJoinSessionComplate(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
```

- 각각의 세션 처리 단계가 끝났을 때 실행되며,  
  로그 출력 또는 레벨 이동 등을 처리함

---

## ✅ 요약

- `ASHGamemode`는 Steam 세션 처리를 직접 다루는 **멀티플레이용 GameMode**입니다.  
- Subsystem이 아닌 `OnlineSessionInterface`를 통해 세션 흐름을 제어하며,  
- 세션 생성 → 검색 → 참가까지 전체를 GameMode 레벨에서 관리할 수 있도록 구성되어 있습니다.
