// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolableLifeTimeComponent.h"

#include "ObjectPool/Subsystems/ObjectPoolSubsystem.h"

void UPoolableLifeTimeComponent::StartTimer()
{
	if (LifeTime > 0.f)
		GetWorld()->GetTimerManager().SetTimer(LifeTimeHandle, this, &ThisClass::OnLifeTimeEnd,LifeTime, false);
}

void UPoolableLifeTimeComponent::StopTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(LifeTimeHandle);
}

void UPoolableLifeTimeComponent::OnLifeTimeEnd()
{
	AActor* Owner = GetOwner();
	if (IsValid(Owner))
		GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->ReturnObjectToPool(Owner);
}
