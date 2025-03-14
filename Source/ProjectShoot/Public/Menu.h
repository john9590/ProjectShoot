﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Menu.generated.h"

UCLASS()
class PROJECTSHOOT_API UMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/Maps/Highrise")));

protected:
	virtual bool Initialize() override;		// Call After UGameInstance Initialize
	virtual void NativeDestruct() override; // Call when remove from parent

	// ----------------------------------------------------------------------
	// Callbacks for the custom delegate on the MultiPlayerSessionSubsystem
	// ----------------------------------------------------------------------
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bwasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);


private:
	class USHSubsystem* MultiplayerSessionSubsystem; // The Subsystem designed to handle all online session functionality

	int32	NumPublicConnections{ 4 };
	FString MatchType{ TEXT("FreeForAll") };
	FString PathToLevel{ TEXT("/Game/Maps/Highrise") };


	UPROPERTY(meta = (BindWidget)) // 블루프린트의 위젯과 변수 연결. (주의! 위젯에 있는 버튼 이름과 같은 변수명이어야 한다)
		class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	// NativeDestruct()->MenuTearDown()
	void MenuTearDown();
};