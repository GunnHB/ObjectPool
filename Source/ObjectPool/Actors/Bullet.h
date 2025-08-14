// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPool/Interfaces/PoolableInterface.h"
#include "Bullet.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UPoolableComponent;

UCLASS()
class OBJECTPOOL_API ABullet : public AActor, public IPoolableInterface
{
	GENERATED_BODY()

public:
	ABullet();

	virtual void OnPoolActivate() override;
	virtual void OnPoolDeactivate() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> CollisionComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPoolableComponent> PoolableComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent = nullptr;

public:
	void FireInDirection(const FVector& ShootDirection);
};
