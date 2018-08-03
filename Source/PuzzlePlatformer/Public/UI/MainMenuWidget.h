// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMER_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:

protected:
	virtual bool Initialize();


private:

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;


	UFUNCTION()
		void HostServer();
};
