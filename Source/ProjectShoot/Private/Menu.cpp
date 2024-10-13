// Fill out your copyright notice in the Description page of Project Settings.
#include "Menu.h"
#include "Components/Button.h"
#include "SHSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

// Lobby ���� ����� ���� �������Ʈ�� ���� ����Ǹ� SHSubsystem�� Session���� Delegate�� �Լ��� ���ε��ؼ� ���� ����� �� �ֵ��� ���ش�.
void UMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	PathToLevel = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<USHSubsystem>();
	}
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		MultiplayerSessionSubsystem->MultiplayerOnFindSessionComplete.AddUObject(this, &ThisClass::OnFindSession);
		MultiplayerSessionSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
		MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
	}
}


bool UMenu::Initialize()
{
	if (!Super::Initialize())
		return false;
	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}
	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}

	return true;
}

void UMenu::NativeDestruct()
{
	MenuTearDown();
	Super::NativeDestruct();
}

// SHSubsystem�� CreateSession�� �Ϸ�Ǹ� ����Ǵ� �Լ��� �� �÷��� Level�� �Ѿ�� �ȴ�
void UMenu::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful) {
		UWorld* World = GetWorld();
		if (World) {
			World->ServerTravel(PathToLevel);
		}
	}
	else {
		HostButton->SetIsEnabled(true);
	}
}

// SHSubsystem�� FindSession�� �Ϸ�Ǹ� ����Ǵ� �Լ��� ������� ������ ��� �� �ϳ��� ��ġŸ���� �´ٸ� JoinSession�� ���� ����� �Ѱ��ش�.
void UMenu::OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bwasSuccessful)
{
	if (MultiplayerSessionSubsystem == nullptr)
		return;
	for (auto Result : SessionResults)
	{
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		if (SettingsValue == MatchType)
		{
			MultiplayerSessionSubsystem->JoinSession(Result);
			return;
		}
	}
	if (!bwasSuccessful || SessionResults.Num() == 0)
		JoinButton->SetIsEnabled(true);
}

// SHSubsystem�� JoinSession�� �Ϸ�Ǹ� ����Ǵ� �Լ��� ����� ������ �ش� ���ǿ� �÷��̾ �� ��Ƽ �÷��̸� �� �� �ֵ��� ���ش�.
void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			if (SessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
			{
				APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
				if (PlayerController)
				{
					PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
				}
			}
		}
	}

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		JoinButton->SetIsEnabled(true);
	}
}

void UMenu::HostButtonClicked()
{
	HostButton->SetIsEnabled(false);
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->CreateSession(NumPublicConnections, MatchType);
	}
}

void UMenu::JoinButtonClicked()
{
	JoinButton->SetIsEnabled(false);

	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->FindSession(10000);
	}
}

void UMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}