// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ObjectPool/Components/PoolableComponent.h"

#include "ObjectPool/DebugHelper.h"
#include "ObjectPool/Components/PoolableLifeTimeComponent.h"

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

	PoolableLifeTimeComponent = CreateDefaultSubobject<UPoolableLifeTimeComponent>(TEXT("LifeTimeComponent"));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(MeshComponent);
	ProjectileMovementComponent->InitialSpeed = 500.0f;
	ProjectileMovementComponent->MaxSpeed = 500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void ABullet::OnPoolActivate(const FGameplayTag& InGameplayTag)
{
	// const FString Msg = FString::Printf(TEXT("%s Activate"), *GetActorNameOrLabel());
	// Debug::Print(Msg, FColor::Green);
	
	if (IsValid(PoolableComponent))
		PoolableComponent->OnActivate(InGameplayTag);

	if (IsValid(PoolableLifeTimeComponent))
		PoolableLifeTimeComponent->StartTimer();
}

void ABullet::OnPoolDeactivate()
{
	// const FString Msg = FString::Printf(TEXT("%s Deactivate"), *GetActorNameOrLabel());
	// Debug::Print(Msg, FColor::Red);

	if (IsValid(PoolableComponent))
		PoolableComponent->OnDeactivate();

	if (IsValid(PoolableLifeTimeComponent))
		PoolableLifeTimeComponent->StopTimer();

	if (IsValid(ProjectileMovementComponent))
	{
		ProjectileMovementComponent->StopMovementImmediately();
		ProjectileMovementComponent->Velocity = FVector::ZeroVector;
	}
}

void ABullet::FireInDirection(const FVector& ShootDirection)
{
	if (IsValid(ProjectileMovementComponent) == false)
		return;

	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}
