// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingObstacle.generated.h"

UCLASS()
class FPPLATFORMER_API ARotatingObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatingObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanDamagePlayer{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float rotSpeed{ 25.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float damage{ 10.f };

	UFUNCTION(BlueprintCallable)
		void DamagePlayer();

	UFUNCTION(BlueprintImplementableEvent)
		void OnPlayerDamaged(float amount);
};
