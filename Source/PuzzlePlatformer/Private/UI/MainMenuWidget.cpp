// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Components/Button.h"



bool UMainMenuWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)return false;

	HostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HostServer);
	return true;
}

void UMainMenuWidget::HostServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Im gonna host a server"));

}
