// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"


#define EPSILON 0.1

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);

	SetReplicates(true);
	SetReplicateMovement(true);

}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	TargetLocation += StartLocation;

	JourneyLength = (TargetLocation - StartLocation).Size();
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector location = GetActorLocation();
	if (!bReverse) {
		location += FVector(TargetLocation - StartLocation).GetSafeNormal()*Speed*DeltaTime;

	}
	else
	{
		location += FVector(TargetLocation - StartLocation).GetSafeNormal()*Speed*DeltaTime;
	}

	JourneyTravelled = (location - StartLocation).Size();
	if (JourneyTravelled >= JourneyLength) {
		bReverse = true;

		FVector temp = StartLocation;
		StartLocation = TargetLocation;
		TargetLocation = temp;
	}


	SetActorLocation(location);

}
