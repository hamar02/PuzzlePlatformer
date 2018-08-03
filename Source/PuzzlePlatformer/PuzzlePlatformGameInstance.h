// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "PuzzlePlatformGameInstance.generated.h"

class FOnlineSessionSearch;
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMER_API UPuzzlePlatformGameInstance : public UGameInstance
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

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr <FOnlineSessionSearch> SessionSearch;
	TSubclassOf<class UUserWidget> MainMenuClass;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);

	void CreateSession();
};
