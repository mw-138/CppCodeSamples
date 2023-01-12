// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingObstacle.h"
#include "FPCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARotatingObstacle::ARotatingObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARotatingObstacle::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARotatingObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float pitch = rotSpeed * DeltaTime;
	FRotator rot = FRotator(0, pitch, 0);
	AddActorLocalRotation(rot);
}

void ARotatingObstacle::DamagePlayer()
{
	AFPCharacter* player = Cast<AFPCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player != nullptr)
	{
		player->Damage(damage);
		bCanDamagePlayer = false;
		OnPlayerDamaged(damage);
	}
}