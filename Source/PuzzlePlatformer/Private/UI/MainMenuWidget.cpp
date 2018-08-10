// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "Blueprint/WidgetTree.h"


void UMainMenuWidget::SetMenuInterface(IMenuInterface* MenuInterface)
{
	this->MenuInterface = MenuInterface;
}

void UMainMenuWidget::Setup()
{
	this->AddToViewport();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		FInputModeUIOnly IM;
		IM.SetWidgetToFocus(this->TakeWidget());
		IM.SetLockMouseToViewportBehavior((EMouseLockMode::DoNotLock));
		PlayerController->SetInputMode(IM);
		PlayerController->bShowMouseCursor = true;
	}

}

void UMainMenuWidget::Teardown()
{
	this->RemoveFromViewport();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		FInputModeGameOnly IM;
		IM.SetConsumeCaptureMouseDown(true);
		PlayerController->SetInputMode(IM);
		PlayerController->bShowMouseCursor = false;
	}
}

void UMainMenuWidget::AddToServerList(UWidget* ServerEntry)
{
	if (ServerList)
	{
		ServerList->AddChild(ServerEntry);
	}
}

bool UMainMenuWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)return false;

	HostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HostServer);
	RefreshServerListButton->OnClicked.AddDynamic(this, &UMainMenuWidget::RefreshServerList);
	FindServerButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenServerMenu);
	BackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenMainMenu);




	return true;
}

void UMainMenuWidget::HostServer()
{
	if (MenuInterface) {
		MenuInterface->Host();
	}

}

void UMainMenuWidget::RefreshServerList()
{
	if (MenuInterface) {
		TArray<UWidget*> ServerEntries;
		this->WidgetTree->GetChildWidgets(ServerList, ServerEntries);
		
		for (UWidget* child : ServerEntries)
		{
			child->RemoveFromParent();
		}

		MenuInterface->RefreshServerList();
	}
}

void UMainMenuWidget::OpenServerMenu()
{
	if (MenuInterface) {
		if (MenuSwitcher) {
			if (ServerMenu)
			{
				MenuSwitcher->SetActiveWidget(ServerMenu);
			}
		}
	}
}

void UMainMenuWidget::OpenMainMenu()
{
	if (MenuInterface) {

		if (MenuSwitcher) {
			if (MainMenu)
			{
				MenuSwitcher->SetActiveWidget(MainMenu);
			}
		}
	}
}
