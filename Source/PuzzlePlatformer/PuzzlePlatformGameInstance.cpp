// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

const static FName SESSION_NAME = ("My Session");

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer & ObjectInitializer)
{
	//Menus
	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_MainMenu(TEXT("/Game/UI/WBP_MainMenu"));
	if (!WBP_MainMenu.Class)
	{
		UE_LOG(LogTemp, Warning, TEXT("WBP_MainMenu Wasn't Found!"));
		return;
	}

	MainMenuClass = WBP_MainMenu.Class;
}

void UPuzzlePlatformGameInstance::Init()
{
	auto OSS =IOnlineSubsystem::Get();
	if (OSS) {
		UE_LOG(LogTemp, Warning, TEXT("OSS is %s"), *OSS->GetSubsystemName().ToString());
		SessionInterface = OSS->GetSessionInterface();
		if (SessionInterface) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnFindSessionComplete);

		}		
	}



}

void UPuzzlePlatformGameInstance::SetupMainMenu()
{

	if (!MainMenuClass) return;

	UUserWidget* MainMenu = CreateWidget<UUserWidget>(this, MainMenuClass);

	MainMenu->AddToViewport();

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController) {
		FInputModeUIOnly IM;
		IM.SetWidgetToFocus(MainMenu->TakeWidget());
		IM.SetLockMouseToViewportBehavior((EMouseLockMode::DoNotLock));
		PlayerController->SetInputMode(IM);
		PlayerController->bShowMouseCursor = true;
	}
}

void UPuzzlePlatformGameInstance::Host()
{
	if (SessionInterface.IsValid()) {
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession) {
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzlePlatformGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;
		UE_LOG(LogTemp, Warning, TEXT("Starting to find session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (Success) {
		UWorld* World = GetWorld();
		if (!World)
		{
			return;
		}

		World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create session"));
	}
}

void UPuzzlePlatformGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success) {
		CreateSession();
	}
}

void UPuzzlePlatformGameInstance::OnFindSessionComplete(bool Success)
{
	if (Success && SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished find session"));
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr());
		}


	}
}

void UPuzzlePlatformGameInstance::CreateSession()
{
	if (SessionInterface.IsValid()) {
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);

	}
}
