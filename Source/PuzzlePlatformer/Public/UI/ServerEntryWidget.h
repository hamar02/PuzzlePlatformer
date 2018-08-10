// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerEntryWidget.generated.h"

/**
 * 
 */


UCLASS()
class PUZZLEPLATFORMER_API UServerEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOnlineSessionSearchResult(uint32 OnlineSessionSearchResultIndex);
	void SetServerNameText(FText ServerName);

protected:
	virtual bool Initialize();

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* JoinServerButton;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerNameText;

	UFUNCTION()
	void JoinServer();
	uint32 OnlineSessionSearchResultIndex;
	
};
