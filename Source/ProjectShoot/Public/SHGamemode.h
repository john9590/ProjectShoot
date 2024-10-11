// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Templates/SharedPointer.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SHGamemode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSHOOT_API ASHGamemode : public AGameMode
{
	GENERATED_BODY()
public:
	ASHGamemode();
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccssful);
	void CreateGameSession();
private:
	IOnlineSessionPtr OnlineSessionInterface;
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
};
