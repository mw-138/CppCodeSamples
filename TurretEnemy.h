// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretEnemy.generated.h"

UCLASS()
class FPPLATFORMER_API ATurretEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurretEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void LookAtPlayer(float deltaTime);
	virtual void Fire();
	FRotator defaultRot;
	float timeSinceLastHit;
	float timeSinceLastShot;
	float provokeTimeout = 5.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Properties

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vitals")
		float currentHealth{ 100.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vitals")
		float maxHealth{ 100.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float damage{ 5.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float fireRate{ .5f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float projectileSpeed{ 10.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
		float detectionRange{ 800.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
		float lookAtSpeed{ 10.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
		bool bPlayerInside{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
		bool bIsProvoked{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Progression")
		float deathBaseExpAmount{ 10.f };

	// Functions

	UFUNCTION(BlueprintCallable, Category = "Detection")
		bool IsPlayerWithinRange();

	UFUNCTION(BlueprintCallable, Category = "Activation")
		void Activate();

	UFUNCTION(BlueprintCallable, Category = "Activation")
		void Deactivate();

	UFUNCTION(BlueprintCallable, Category = "Vitals")
		void InflictDamage(float amount);

	UFUNCTION(BlueprintCallable, Category = "Detection")
		void Provoke();

	// Events

	UFUNCTION(BlueprintImplementableEvent, Category = "Detection")
		void OnPlayerEnter();

	UFUNCTION(BlueprintImplementableEvent, Category = "Detection")
		void OnPlayerExit();

	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
		void OnFire();

	UFUNCTION(BlueprintImplementableEvent, Category = "Activation")
		void OnActivation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Activation")
		void OnDeactivation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Vitals")
		void OnDestroyed();

	UFUNCTION(BlueprintImplementableEvent, Category = "Detection")
		void OnProvoked();

};
