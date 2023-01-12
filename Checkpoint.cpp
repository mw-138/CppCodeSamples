// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint.h"
#include "FPCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	spawnPos = GetActorLocation();
	spawnRot = GetActorRotation();
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACheckpoint::UpdatePlayerCheckpoint()
{
	AFPCharacter* player = Cast<AFPCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player != nullptr)
	{
		player->UpdateCheckpoint(spawnPos, spawnRot);
		Destroy();
	}
}

