// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/MenuInterface.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMER_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetMenuInterface(IMenuInterface* MenuInterface);
	void Setup();
	void Teardown();

	void AddToServerList(UWidget* ServerEntry);
protected:
	virtual bool Initialize();


private:
	//Menu switcher
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	//Main menu
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* FindServerButton;

	//Server Menu
	UPROPERTY(meta = (BindWidget))
	class UWidget* ServerMenu;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* RefreshServerListButton;


//Main Menu
	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void OpenServerMenu();

//Server Menu
	UFUNCTION()
		void RefreshServerList();

	UFUNCTION()
		void OpenMainMenu();


	IMenuInterface* MenuInterface;
};
