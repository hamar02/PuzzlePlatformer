// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "PuzzlePlatformGameInstance.h"


void UServerEntryWidget::SetOnlineSessionSearchResult(uint32 OnlineSessionSearchResultIndex)
{
	this->OnlineSessionSearchResultIndex = OnlineSessionSearchResultIndex;
}

void UServerEntryWidget::SetServerNameText(FText ServerName)
{
	if (ServerNameText)
	{
		ServerNameText->SetText(ServerName);
	}
}

bool UServerEntryWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)return false;

	JoinServerButton->OnClicked.AddDynamic(this, &UServerEntryWidget::JoinServer);




	return true;
}

void UServerEntryWidget::JoinServer()
{
	UPuzzlePlatformGameInstance* GI = Cast<UPuzzlePlatformGameInstance>(GetGameInstance());
	if (GI) {
		GI->Join(OnlineSessionSearchResultIndex);
		UE_LOG(LogTemp, Warning, TEXT("Clicked join server, server index is %d"), OnlineSessionSearchResultIndex);

	}
}
