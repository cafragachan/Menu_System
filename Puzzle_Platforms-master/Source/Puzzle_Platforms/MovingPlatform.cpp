// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);

}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
		WorldTargetLocation = GetTransform().TransformPosition(TargetLocation);
		WorldStartLocation = GetActorLocation();
	}
}

void  AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && bIsActivated)
	{
		FVector Location = GetActorLocation();
		float JourneyTravelled = (WorldStartLocation - Location).Size();
		float JourneyLength = (WorldStartLocation - WorldTargetLocation).Size();

		if (JourneyTravelled >= JourneyLength)
		{
			FVector SwapLocation = WorldStartLocation;
			WorldStartLocation = WorldTargetLocation;
			WorldTargetLocation = SwapLocation;
		}

		FVector Direction = WorldTargetLocation - Location;
		Direction = Direction.GetSafeNormal() * DeltaTime * MoveSpeed;
		this->SetActorLocation(Location + Direction);

	}

}

void AMovingPlatform::SetMobility(bool isActive)
{
	isActive ? bIsActivated = true : bIsActivated = false;
}
