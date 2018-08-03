// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMER_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	AMovingPlatform();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	bool bReverse = true;
	FVector StartLocation;

	UPROPERTY(EditAnywhere, meta=(MakeEditWidget=true), Category="Options")
		FVector TargetLocation;

	UPROPERTY(EditAnywhere, Category = "Options")
		float Speed = 1;

private:
	float JourneyLength = 0;
	float JourneyTravelled = 0;
};
