// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformGameInstance.h"
#include "OnlineSessionSettings.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "UI/MainMenuWidget.h"
#include "UI/ServerEntryWidget.h"


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

	//Server Entry
	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_ServerEntry(TEXT("/Game/UI/WBP_ServerEntry"));
	if (!WBP_ServerEntry.Class)
	{
		UE_LOG(LogTemp, Warning, TEXT("WBP_ServerEntry Wasn't Found!"));
		return;
	}

	ServerEntryClass = WBP_ServerEntry.Class;
}

void UPuzzlePlatformGameInstance::Init()
{
	auto OSS =IOnlineSubsystem::Get();
	if (OSS) {
		UE_LOG(LogTemp, Warning, TEXT("Online Subsystem is %s"), *OSS->GetSubsystemName().ToString());
		SessionInterface = OSS->GetSessionInterface();
		if (SessionInterface) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnJoinSessionComplete);

		}		
	}



}

void UPuzzlePlatformGameInstance::SetupMainMenu()
{

	if (!MainMenuClass) return;

	MainMenu = CreateWidget<UMainMenuWidget>(this, MainMenuClass);
	if (!MainMenu)return;

	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::Host()
{
	if (MainMenu)
	{
		MainMenu->Teardown();
	}

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
		UE_LOG(LogTemp, Warning, TEXT("Starting to find session"));
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformGameInstance::Join(uint32 OnlineSessionSearchResultIndex)
{
	if (!SessionInterface.IsValid())return;
	if (!SessionSearch.IsValid())return;

	if (MainMenu) {
		MainMenu->Teardown();
	}
	UE_LOG(LogTemp, Warning, TEXT("gggggggggg"));

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[OnlineSessionSearchResultIndex]);
}

void UPuzzlePlatformGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (Success) {
		UWorld* World = GetWorld();
		if (!World)
		{
			return;
		}

		World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create session"));
	}
}


void UPuzzlePlatformGameInstance::OnFindSessionComplete(bool Success)
{
	if (MainMenu)
	{
		if (Success && SessionSearch.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Finished find session"));
			int8 i= 0;
			for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
			{
				if (!ServerEntryClass) return;
				UServerEntryWidget* ServerEntry = CreateWidget<UServerEntryWidget>(this, ServerEntryClass);
				ServerEntry->SetServerNameText(FText::FromString(SearchResult.Session.GetSessionIdStr()));
				ServerEntry->SetOnlineSessionSearchResult(i);
				MainMenu->AddToServerList(ServerEntry);
				UE_LOG(LogTemp, Warning, TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr());
				i++;
			}


		}
	}
}

void UPuzzlePlatformGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success) {
		CreateSession();
	}
}

void UPuzzlePlatformGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{

	if (!SessionInterface)return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Resolved Connection Address is %s"), *Address);

	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC) {
		PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);

	}


}

void UPuzzlePlatformGameInstance::CreateSession()
{
	if (SessionInterface.IsValid()) {
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = false;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true; 
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);

	}
}
