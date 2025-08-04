// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolableComponent.h"

UPoolableComponent::UPoolableComponent()
{
	SetIsReplicatedByDefault(true);
}

void UPoolableComponent::OnActivate()
{
	AActor* Owner = GetOwner();
	if (IsValid(Owner) == false)
		return;

	bIsActivate = true;

	Owner->SetActorTickEnabled(true);
	Owner->SetActorHiddenInGame(false);
	Owner->SetActorEnableCollision(true);
}

void UPoolableComponent::OnDeactivate()
{
	AActor* Owner = GetOwner();
	if (IsValid(Owner) == false)
		return;

	bIsActivate = false;

	Owner->SetActorTickEnabled(false);
	Owner->SetActorHiddenInGame(true);
	Owner->SetActorEnableCollision(false);
}
