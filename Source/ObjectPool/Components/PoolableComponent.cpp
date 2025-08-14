// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolableComponent.h"

#include "ObjectPool/Subsystems/ObjectPoolSubsystem.h"

UPoolableComponent::UPoolableComponent()
{
	SetIsReplicatedByDefault(true);
}

void UPoolableComponent::OnActivate(const FGameplayTag& InGameplayTag)
{
	AActor* Owner = GetOwner();
	if (IsValid(Owner) == false)
		return;

	Tag = InGameplayTag;
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
	
	Tag = FGameplayTag::EmptyTag;
	bIsActivate = false;

	Owner->SetActorTickEnabled(false);
	Owner->SetActorHiddenInGame(true);
	Owner->SetActorEnableCollision(false);
}
