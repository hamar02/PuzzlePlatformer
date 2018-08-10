// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "UI/MenuInterface.h"
#include "PuzzlePlatformGameInstance.generated.h"

class FOnlineSessionSearch;
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMER_API UPuzzlePlatformGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
public:
	UPuzzlePlatformGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Network")
		void SetupMainMenu();

	UFUNCTION(BlueprintCallable, Category = "Network")
		void Host();


	UFUNCTION(BlueprintCallable, Category = "Network")
		void RefreshServerList();

	void Join(uint32 OnlineSessionSearchResultIndex);

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr <FOnlineSessionSearch> SessionSearch;
	TSubclassOf<class UUserWidget> MainMenuClass;
	TSubclassOf<class UUserWidget> ServerEntryClass;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void CreateSession();
	class UMainMenuWidget* MainMenu;
};
