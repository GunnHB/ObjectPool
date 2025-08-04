// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/SphereComponent.h"
#include "ObjectPool/Components/PoolableComponent.h"

ABullet::ABullet()
{
	bReplicates = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));;
	CollisionComponent->SetupAttachment(MeshComponent);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PoolableComponent = CreateDefaultSubobject<UPoolableComponent>(TEXT("PoolableComponent"));
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

void ABullet::Destroyed()
{
	UE_LOG(LogTemp, Error, TEXT("Destroy!!"));
	
	Super::Destroyed();
}
