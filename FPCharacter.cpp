// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TurretEnemy.h"

// Sets default values
AFPCharacter::AFPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();

	checkpointPos = GetActorLocation();
	checkpointRot = GetActorRotation();
	
	UCharacterMovementComponent* movementComp = Cast<UCharacterMovementComponent>(GetCharacterMovement());
	if (movementComp != nullptr)
	{
		movementComp->AirControl = 1.f;
	}
}

// Called every frame
void AFPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	deltaTime = DeltaTime;

	HandleWeaponShooting();
}

// Called to bind functionality to input
void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForwards", this, &AFPCharacter::MoveForwards);
	PlayerInputComponent->BindAxis("MoveBackwards", this, &AFPCharacter::MoveBackwards);
	PlayerInputComponent->BindAxis("MoveLeft", this, &AFPCharacter::MoveLeft);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookHorizontal", this, &AFPCharacter::LookHorizontal);
	PlayerInputComponent->BindAxis("LookVertical", this, &AFPCharacter::LookVertical);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPCharacter::Jump);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPCharacter::BeginSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPCharacter::EndSprint);
}

void AFPCharacter::SetCanMove(bool bState)
{
	bCanMove = bState;
}

void AFPCharacter::MoveForwards(float value)
{
	if (!bCanMove) return;
	FVector worldDirection = GetActorForwardVector();
	AddMovementInput(worldDirection, value, false);
}

void AFPCharacter::MoveBackwards(float value)
{
	if (!bCanMove) return;
	FVector worldDirection = GetActorForwardVector();
	AddMovementInput(worldDirection, value, false);
}

void AFPCharacter::MoveLeft(float value)
{
	if (!bCanMove) return;
	FVector worldDirection = GetActorRightVector();
	AddMovementInput(worldDirection, value, false);
}

void AFPCharacter::MoveRight(float value)
{
	if (!bCanMove) return;
	FVector worldDirection = GetActorRightVector();
	AddMovementInput(worldDirection, value, false);
}

void AFPCharacter::LookHorizontal(float value)
{
	AddControllerYawInput(value);
}

void AFPCharacter::LookVertical(float value)
{
	AddControllerPitchInput(value * -1.0f);
}

void AFPCharacter::BeginSprint()
{
	bIsSprinting = true;
	UCharacterMovementComponent* movementComp = Cast<UCharacterMovementComponent>(GetCharacterMovement());
	if (movementComp != nullptr)
	{
		movementComp->MaxWalkSpeed = sprintSpeed;
	}
}

void AFPCharacter::EndSprint()
{
	bIsSprinting = false;
	UCharacterMovementComponent* movementComp = Cast<UCharacterMovementComponent>(GetCharacterMovement());
	if (movementComp != nullptr)
	{
		movementComp->MaxWalkSpeed = walkSpeed;
	}
}

void AFPCharacter::HandleWeaponShooting()
{
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (playerController != nullptr)
	{
		bool bFireKeyDown = playerController->IsInputKeyDown(EKeys::LeftMouseButton);
		if (bFireKeyDown)
		{
			timeSinceLastShot += 1.f * deltaTime;
			if (timeSinceLastShot >= weaponFireRate)
			{
				timeSinceLastShot = 0.f;

				float spreadMultiplier = weaponSpreadIntensity / 3.f;
				float pitch = FMath::RandRange(weaponSpreadPitch.X, weaponSpreadPitch.Y) * spreadMultiplier;
				float yaw = FMath::RandRange(weaponSpreadYaw.X, weaponSpreadYaw.Y) * spreadMultiplier;
				AddControllerPitchInput(pitch);
				AddControllerYawInput(yaw);

				FHitResult hit = Raycast(false, weaponRange);

				int critChance = FMath::RandRange(0, 100); // rand() % 100;
				bool bCritHit = critChance <= 50;
				float dmg = bCritHit ? weaponCritDamage : weaponBaseDamage;
				bool bDidHitTarget = false;
				bool bDidKill = false;

				AActor* hitActor = hit.GetActor();
				if (hitActor != nullptr)
				{
					ATurretEnemy* turret = Cast<ATurretEnemy>(hitActor);
					if (turret != nullptr)
					{
						bDidHitTarget = true;
						turret->InflictDamage(dmg);
						if (turret->currentHealth <= 0.f)
						{
							bDidKill = true;
						}
					}
				}

				OnWeaponFired(hit, dmg, bCritHit, bDidHitTarget, bDidKill);
			}
		}
	}
}

void AFPCharacter::SetCurrentHealth(float newHealth)
{
	if (newHealth < currentHealth)
	{
		OnHealthLost(newHealth);
	}
	else
	{
		OnHealthGained(newHealth);
	}
	currentHealth = newHealth;
}

void AFPCharacter::AddHealth(float amount)
{
	currentHealth += amount;
	if (currentHealth >= maxHealth)
	{
		currentHealth = maxHealth;
	}
	OnHealthGained(amount);
}

void AFPCharacter::TakeHealth(float amount)
{
	currentHealth -= amount;
	if (currentHealth <= 0)
	{
		currentHealth = 0;
		OnPlayerDeath();
	}
	OnHealthLost(amount);
}

void AFPCharacter::SetMaxHealth(float newHealth)
{
	if (newHealth < maxHealth)
	{
		OnMaxHealthIncreased(newHealth);
	}
	else
	{
		OnMaxHealthDecreased(newHealth);
	}
	maxHealth = newHealth;
}

void AFPCharacter::IncreaseMaxHealth(float amount)
{
	maxHealth += amount;
	OnMaxHealthIncreased(amount);
}

void AFPCharacter::DecreaseMaxHealth(float amount)
{
	maxHealth -= amount;
	if (maxHealth < minMaxHealth)
	{
		maxHealth = minMaxHealth;
	}
	OnMaxHealthDecreased(amount);
}

void AFPCharacter::SetCurrentArmor(float newArmor)
{
	if (newArmor < currentArmor)
	{
		OnArmorGained(newArmor);
	}
	else
	{
		OnArmorLost(newArmor);
	}
	currentArmor = newArmor;
	OnArmorGained(newArmor);
}

void AFPCharacter::AddArmor(float amount)
{
	currentArmor += amount;
	if (currentArmor >= maxArmor)
	{
		currentArmor = maxArmor;
	}
	OnArmorGained(amount);
}

void AFPCharacter::TakeArmor(float amount)
{
	currentArmor -= amount;
	OnArmorLost(amount);
}

void AFPCharacter::SetMaxArmor(float newArmor)
{
	if (newArmor < maxArmor)
	{
		OnMaxArmorIncreased(newArmor);
	}
	else
	{
		OnMaxArmorDecreased(newArmor);
	}
	maxArmor = newArmor;
}

void AFPCharacter::IncreaseMaxArmor(float amount)
{
	maxArmor += amount;
	OnMaxArmorIncreased(amount);
}

void AFPCharacter::DecreaseMaxArmor(float amount)
{
	maxArmor -= amount;
	if (maxArmor < minMaxArmor)
	{
		maxArmor = minMaxArmor;
	}
	OnMaxArmorDecreased(amount);
}

void AFPCharacter::Damage(float amount)
{
	if (currentArmor > 0)
	{
		TakeArmor(amount);
		if (currentArmor < 0)
		{
			float const overflow = abs(currentArmor);
			SetCurrentArmor(0);
			TakeHealth(overflow);
		}
	}
	else
	{
		TakeHealth(amount);
	}
}

FHitResult AFPCharacter::Raycast(bool bDebug, float range)
{
	FVector rayStart;
	FVector rayEnd = FVector::ZeroVector;
	FRotator rayRot;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(rayStart, rayRot);
	rayEnd = rayStart + (rayRot.Vector() * range);

	FCollisionQueryParams params(SCENE_QUERY_STAT(GetRaycastHit), true, GetInstigator());
	FHitResult hit(ForceInit);
	bool bHit = GetWorld()->LineTraceSingleByChannel(hit, rayStart, rayEnd, ECC_Visibility, params);

	if (bDebug)
	{
		DrawDebugLine(GetWorld(), rayStart, rayEnd, FColor::Red, false, 2.0f);
		if (bHit)
		{
			DrawDebugBox(GetWorld(), hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
		}
	}

	return hit;
}

void AFPCharacter::SetTotalCoins(int amount)
{
	if (amount < totalCoins)
	{
		OnCoinsReceived(amount);
	}
	else
	{
		OnCoinsTaken(amount);
	}
	totalCoins = amount;
}

void AFPCharacter::AddCoins(int amount)
{
	if (totalCoins > maxCoins)
	{
		totalCoins = maxCoins;
	}
	else
	{
		totalCoins += amount;
		OnCoinsReceived(amount);
	}
}

void AFPCharacter::TakeCoins(int amount)
{
	if (totalCoins < 0)
	{
		totalCoins = 0;
	}
	else
	{
		totalCoins -= amount;
		OnCoinsTaken(amount);
	}
}

bool AFPCharacter::HasEnoughCoins(int amount)
{
	return totalCoins <= amount;
}

void AFPCharacter::SetTotalGems(int amount)
{
	if (amount < totalGems)
	{
		OnGemsReceived(amount);
	}
	else
	{
		OnGemsTaken(amount);
	}
	totalGems = amount;
}

void AFPCharacter::AddGems(int amount)
{
	if (totalGems > maxGems)
	{
		totalGems = maxGems;
	}
	else
	{
		totalGems += amount;
		OnGemsReceived(amount);
	}
}

void AFPCharacter::TakeGems(int amount)
{
	if (totalGems < 0)
	{
		totalGems = 0;
	}
	else
	{
		totalGems -= amount;
		OnGemsTaken(amount);
	}
}

bool AFPCharacter::HasEnoughGems(int amount)
{
	return totalGems <= amount;
}

void AFPCharacter::UpdateCheckpoint(FVector pos, FRotator rot)
{
	checkpointPos = pos;
	checkpointRot = rot;
	OnCheckpointReached();
}

void AFPCharacter::Respawn()
{
	FVector pos = checkpointPos;
	FRotator rot = checkpointRot;
	SetActorLocation(pos);
	SetActorRotation(rot);
	OnRespawn(pos, rot);
}

void AFPCharacter::ToggleFlashlight()
{
	bFlashlightActive = !bFlashlightActive;
	OnFlashlightToggled(!bFlashlightActive);
}

int AFPCharacter::GetNextLevel()
{
	return currentLevel + 1;
}

int AFPCharacter::GetPreviousLevel()
{
	return currentLevel - 1;
}

float AFPCharacter::GetExpExponent(int level, float baseExp)
{
	float const exponent = 1.5f;
	float const exp = FMath::Floor(baseExp * pow(level, exponent));
	return exp;
}

float AFPCharacter::GenerateExp(float baseExp)
{
	float const exp = GetExpExponent(currentLevel, baseExp);
	return exp;
}

float AFPCharacter::CalculateMaxExpForLevel(int level)
{
	float const baseExp = 1000.f;
	float const maxExp = GetExpExponent(level, baseExp);
	return maxExp;
}

int AFPCharacter::CalculateLevel(float exp)
{
	int level = 0;
	while (CalculateMaxExpForLevel(level) < exp)
	{
		level++;
	}
	return level;
}

void AFPCharacter::GiveExp(float amount)
{
	float const newAmount = GenerateExp(amount) * bonusExpMultiplier;
	currentExp += newAmount;
	OnExpGained(newAmount);
	float const maxExp = CalculateMaxExpForLevel(GetNextLevel());
	if (currentExp >= maxExp)
	{
		LevelUp();
	}
}

void AFPCharacter::LevelUp()
{
	float const maxExp = CalculateMaxExpForLevel(GetNextLevel());
	float const overflow = abs(maxExp - currentExp);
	currentLevel++;
	currentExp = 0.f;
	if (overflow > 0.f)
	{
		GiveExp(overflow);
	}
	OnLevelUp(currentLevel);
}

void AFPCharacter::CreateDamagePopup(AActor* hitActor, float damage, bool bWasCritHit, bool bDidKill)
{
	OnDamagePopupCreated(hitActor, damage, bWasCritHit, bDidKill);
}

bool AFPCharacter::IsUsingEditor()
{
#if WITH_EDITOR
	return true;
#endif
	return false;
}

bool AFPCharacter::IsDev()
{
	static const FString name = UKismetSystemLibrary::GetPlatformUserName();
	for (FString dev : devNames)
	{
		if (name == dev)
		{
			return true;
		}
	}
	return false;
}