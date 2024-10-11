// Fill out your copyright notice in the Description page of Project Settings.

#include "SHGamemode.h"

ASHGamemode::ASHGamemode()
    : CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete))
{
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Found subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString()));
        }
    }
}

void ASHGamemode::OnCreateSessionComplete(FName SessionName, bool bWasSuccssful)
{
	if (bWasSuccssful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Created session : %s"), *SessionName.ToString()));
		}
	}

	// 세선 생성 실패
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Failed to create session!")));
		}
	}
}

void ASHGamemode::CreateGameSession()
{
	// Called when pressing the 1key
	if (!OnlineSessionInterface.IsValid())
	{
		// log
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Game Session is invailed")));
		}
		return;
	}

	// 이미 세션이 존재한다면 기존 세션을 삭제한다
	auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		OnlineSessionInterface->DestroySession(NAME_GameSession);

		// Log
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, FString::Printf(TEXT("Destroy session : %s"), NAME_GameSession));
		}
	}

	// 세션 생성 완료 후 호출될 delegate 리스트에 CreateSessionCompleteDelegate 추가
	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	// 세션 세팅하기
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());

	SessionSettings->bIsLANMatch = false;			// LAN 연결
	SessionSettings->NumPublicConnections = 4;		// 최대 접속 가능 수
	SessionSettings->bAllowJoinInProgress = true;	// Session 진행중에 접속 허용
	SessionSettings->bAllowJoinViaPresence = true;  // 세션 참가 지역을 현재 지역으로 제한 (스팀의 presence 사용)
	SessionSettings->bShouldAdvertise = true;		// 현재 세션을 광고할지 (스팀의 다른 플레이어에게 세션 홍보 여부)
	SessionSettings->bUsesPresence = true;			// 현재 지역에 세션 표시
	SessionSettings->bUseLobbiesIfAvailable = true; // 플랫폼이 지원하는 경우 로비 API 사용

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
}
