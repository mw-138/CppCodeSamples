// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "FPCharacter.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATurretEnemy::ATurretEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATurretEnemy::BeginPlay()
{
	Super::BeginPlay();

	defaultRot = GetActorRotation();

	Deactivate();
}

// Called every frame
void ATurretEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookAtPlayer(DeltaTime);

	if (bIsProvoked)
	{
		timeSinceLastHit += 1.f * DeltaTime;
		if (timeSinceLastHit >= provokeTimeout)
		{
			bIsProvoked = false;
		}
	}
}

void ATurretEnemy::LookAtPlayer(float deltaTime)
{
	DrawDebugSphere(GetWorld(), GetActorLocation(), detectionRange, 50, FColor::Red, 0.0f, 0.0f);

	if (!IsPlayerWithinRange() && !bIsProvoked) {
		if (bPlayerInside) {
			bPlayerInside = false;
			OnPlayerExit();
		}
		if (GetActorRotation() != defaultRot) {
			FRotator lerpRot = FMath::RInterpTo(GetActorRotation(), defaultRot, deltaTime, lookAtSpeed);
			RootComponent->SetWorldRotation(lerpRot);
		}
		return;
	}

	if (!bPlayerInside) {
		bPlayerInside = true;
		OnPlayerEnter();
	}

	FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FRotator lookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), playerLocation);
	FRotator lerpRot = FMath::RInterpTo(GetActorRotation(), lookAtRot, deltaTime, lookAtSpeed);
	RootComponent->SetWorldRotation(lerpRot);

	timeSinceLastShot += 1.f * deltaTime;
	if (timeSinceLastShot >= fireRate)
	{
		timeSinceLastShot = 0.f;
		OnFire();
	}
}

void ATurretEnemy::Fire()
{
	FVector pos = GetActorLocation();
	FRotator rot = GetActorRotation();
	FActorSpawnParameters spawnInfo;
	spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(pos, rot, spawnInfo);
	if (projectile != nullptr)
	{
		/*projectile->travelSpeed = projectileSpeed;
		projectile->damage = damage;*/
		projectile->UpdateInfo(projectileSpeed, damage);
	}
}

bool ATurretEnemy::IsPlayerWithinRange()
{
	FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector myLocation = GetActorLocation();
	float distFromPlayer = FVector::Dist(myLocation, playerLocation);
	return distFromPlayer <= detectionRange;
}

void ATurretEnemy::Activate()
{
	OnActivation();
}

void ATurretEnemy::Deactivate()
{
	OnDeactivation();
}

void ATurretEnemy::InflictDamage(float amount)
{
	Provoke();
	timeSinceLastHit = 0.f;
	currentHealth -= amount;
	if (currentHealth <= 0)
	{
		AFPCharacter* player = Cast<AFPCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (player != nullptr)
		{
			player->GiveExp(player->GenerateExp(deathBaseExpAmount));
		}
		OnDestroyed();
		Destroy();
	}
}

void ATurretEnemy::Provoke()
{
	if (bIsProvoked) return;
	bIsProvoked = true;
	OnProvoked();
}