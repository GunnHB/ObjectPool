// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ObjectPool/Components/PoolableComponent.h"

ABullet::ABullet()
{
	bReplicates = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(MeshComponent);

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(MeshComponent);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PoolableComponent = CreateDefaultSubobject<UPoolableComponent>(TEXT("PoolableComponent"));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(MeshComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void ABullet::OnPoolActivate()
{
	UE_LOG(LogTemp, Warning, TEXT("OnActivate"));

	if (IsValid(PoolableComponent))
		PoolableComponent->OnActivate();
}

void ABullet::OnPoolDeactivate()
{
	UE_LOG(LogTemp, Warning, TEXT("OnDeactivate"));

	if (IsValid(PoolableComponent))
		PoolableComponent->OnDeactivate();
}

void ABullet::FireInDirection(const FVector& ShootDirection)
{
	if (IsValid(ProjectileMovementComponent) == false)
		return;

	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}
