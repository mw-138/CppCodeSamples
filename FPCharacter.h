// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPCharacter.generated.h"

UCLASS()
class FPPLATFORMER_API AFPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void MoveForwards(float value);
	virtual void MoveBackwards(float value);
	virtual void MoveLeft(float value);
	virtual void MoveRight(float value);
	virtual void LookHorizontal(float value);
	virtual void LookVertical(float value);
	virtual void BeginSprint();
	virtual void EndSprint();
	virtual void HandleWeaponShooting();

private:
	float minMaxHealth{ 100.f };
	float minMaxArmor{ 100.f };
	int maxCoins{ 9999999 };
	int maxGems{ 9999999 };
	FVector checkpointPos;
	FRotator checkpointRot;
	float respawnHeight{ -100.f };
	float walkSpeed{ 600.f };
	float sprintSpeed{ 900.f };
	float timeSinceLastShot;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Properties

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float deltaTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vitals")
		float currentHealth{ 100.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vitals")
		float maxHealth{ 100.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vitals")
		float currentArmor{ 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vitals")
		float maxArmor{ 100.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		bool bCanMove{ true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		bool bIsSprinting{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Currency")
		int totalCoins{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Currency")
		int totalGems{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Raycast")
		float interactRange{ 150.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float weaponRange{ 1300.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float weaponBaseDamage{ 10.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float weaponCritDamage{ 25.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float weaponFireRate{ 0.1f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float weaponSpreadIntensity{ 2.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FVector2D weaponSpreadPitch{ 0.f, -0.25f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FVector2D weaponSpreadYaw{ -0.25f, 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bFlashlightActive{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Levelling")
		int currentLevel{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Levelling")
		float currentExp{ 0.f };

	UPROPERTY(EditAnywhere, Category = "Levelling")
		float bonusExpMultiplier{ 1.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float respawnDamage{ 10.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FString> devNames{ "matth" };

	// Functions

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void SetCanMove(bool bState);

	UFUNCTION(BlueprintCallable, Category = "Vitals")
		void SetCurrentHealth(float newHealth);

	UFUNCTION(BlueprintCallable, Category = "Vitals")
		void AddHealth(float amount);

	UFUNCTION(BlueprintCallable, Category = "Vitals")
		void TakeHealth(float amount);

	UFUNCTION(BlueprintCallable, Category = "Vitals")
		void SetMaxHealth(float newHealth);

	UFUNCTION(BlueprintCallable, Category = "Vitals")
		void IncreaseMaxHealth(float amount);

	UFUNCTION(BlueprintCallable, Category = "Vitals")
		void DecreaseMaxHealth(float amount);

	UFUNCTION(BlueprintCallable, Category = "Vitals")
		void SetCurrentArmor(float newArmor);

	UFUNCTION(BlueprintCallable, Category = "Vitals")
		void AddArmor(float amount);

	UFUNCTION(BlueprintCallable, Category = "Vitals")
		void TakeArmor(float amount);

	UFUNCTION(BlueprintCallable, Category = "Vitals")
		void SetMaxArmor(float newArmor);

	UFUNCTION(BlueprintCallable, Category = "Vitals")
		void IncreaseMaxArmor(float amount);

	UFUNCTION(BlueprintCallable, Category = "Vitals")
		void DecreaseMaxArmor(float amount);

	UFUNCTION(BlueprintCallable, Category = "Vitals")
		void Damage(float amount);

	UFUNCTION(BlueprintCallable, Category = "Raycast")
		FHitResult Raycast(bool bDebug, float range);

	UFUNCTION(BlueprintCallable, Category = "Currency")
		void SetTotalCoins(int amount);

	UFUNCTION(BlueprintCallable, Category = "Currency")
		void AddCoins(int amount);

	UFUNCTION(BlueprintCallable, Category = "Currency")
		void TakeCoins(int amount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Currency")
		bool HasEnoughCoins(int amount);

	UFUNCTION(BlueprintCallable, Category = "Currency")
		void SetTotalGems(int amount);

	UFUNCTION(BlueprintCallable, Category = "Currency")
		void AddGems(int amount);

	UFUNCTION(BlueprintCallable, Category = "Currency")
		void TakeGems(int amount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Currency")
		bool HasEnoughGems(int amount);

	UFUNCTION(BlueprintCallable, Category = "Respawn")
		void UpdateCheckpoint(FVector pos, FRotator rot);

	UFUNCTION(BlueprintCallable, Category = "Respawn")
		void Respawn();

	UFUNCTION(BlueprintCallable)
		void ToggleFlashlight();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Levelling")
		int GetPreviousLevel();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Levelling")
		int GetNextLevel();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Levelling")
		float GetExpExponent(int level, float baseExp);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Levelling")
		float GenerateExp(float baseExp);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Levelling")
		float CalculateMaxExpForLevel(int level);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Levelling")
		int CalculateLevel(float exp);

	UFUNCTION(BlueprintCallable, Category = "Levelling")
		void GiveExp(float amount);

	UFUNCTION(BlueprintCallable, Category = "Levelling")
		void LevelUp();

	UFUNCTION(BlueprintCallable)
		void CreateDamagePopup(AActor* hitActor, float damage, bool bWasCritHit, bool bDidKill);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsUsingEditor();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsDev();

	// Events

	UFUNCTION(BlueprintImplementableEvent, Category = "Vitals")
		void OnHealthGained(float amount);

	UFUNCTION(BlueprintImplementableEvent, Category = "Vitals")
		void OnHealthLost(float amount);

	UFUNCTION(BlueprintImplementableEvent, Category = "Vitals")
		void OnMaxHealthIncreased(float amount);

	UFUNCTION(BlueprintImplementableEvent, Category = "Vitals")
		void OnMaxHealthDecreased(float amount);

	UFUNCTION(BlueprintImplementableEvent, Category = "Vitals")
		void OnArmorGained(float amount);

	UFUNCTION(BlueprintImplementableEvent, Category = "Vitals")
		void OnArmorLost(float amount);

	UFUNCTION(BlueprintImplementableEvent, Category = "Vitals")
		void OnMaxArmorIncreased(float amount);

	UFUNCTION(BlueprintImplementableEvent, Category = "Vitals")
		void OnMaxArmorDecreased(float amount);

	UFUNCTION(BlueprintImplementableEvent, Category = "Vitals")
		void OnPlayerDeath();

	UFUNCTION(BlueprintImplementableEvent, Category = "Currency")
		void OnCoinsReceived(int amount);

	UFUNCTION(BlueprintImplementableEvent, Category = "Currency")
		void OnCoinsTaken(int amount);

	UFUNCTION(BlueprintImplementableEvent, Category = "Currency")
		void OnGemsReceived(int amount);

	UFUNCTION(BlueprintImplementableEvent, Category = "Currency")
		void OnGemsTaken(int amount);

	UFUNCTION(BlueprintImplementableEvent, Category = "Respawn")
		void OnRespawn(FVector pos, FRotator rot);

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
		void OnWeaponFired(FHitResult hit, float inflictedDamage, bool bWasCritHit, bool bDidHitTarget, bool bDidKill);

	UFUNCTION(BlueprintImplementableEvent)
		void OnFlashlightToggled(bool bActive);

	UFUNCTION(BlueprintImplementableEvent, Category = "Levelling")
		void OnLevelUp(int newLevel);

	UFUNCTION(BlueprintImplementableEvent, Category = "Levelling")
		void OnExpGained(float amount);

	UFUNCTION(BlueprintImplementableEvent)
		void OnDamagePopupCreated(AActor* hitActor, float damage, bool bWasCritHit, bool bDidKill);

	UFUNCTION(BlueprintImplementableEvent, Category = "Respawn")
		void OnCheckpointReached();

};
