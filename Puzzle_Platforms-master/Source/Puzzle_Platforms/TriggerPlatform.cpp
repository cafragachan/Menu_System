// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerPlatform.h"

#include "Components/BoxComponent.h"
#include "MovingPlatform.h"

// Sets default values
ATriggerPlatform::ATriggerPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BoxPlatform = CreateDefaultSubobject<UBoxComponent>("Box Platform");
	SetRootComponent(BoxPlatform);

	
}

// Called when the game starts or when spawned
void ATriggerPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	BoxPlatform->OnComponentBeginOverlap.AddDynamic(this, &ATriggerPlatform::OnOverlapActivate);
	BoxPlatform->OnComponentEndOverlap.AddDynamic(this, &ATriggerPlatform::OnOverlapDeactivate);

	for (auto& platform : MovingPlatforms)
	{
		platform->SetMobility(false);
	}
}

// Called every frame
void ATriggerPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerPlatform::OnOverlapActivate(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Activate: %s"), *OtherActor->GetName());

	for (auto& platform : MovingPlatforms)
	{
		platform->SetMobility(true);
	}
}

void ATriggerPlatform::OnOverlapDeactivate(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Deactivate: %s"), *OtherActor->GetName());

	for (auto& platform : MovingPlatforms)
	{
		platform->SetMobility(false);
	}
}

