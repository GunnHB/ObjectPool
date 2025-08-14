// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolableLifeTimeComponent.h"

#include "ObjectPool/DebugHelper.h"


void UPoolableLifeTimeComponent::ActivateLifeTimer()
{
	GetWorld()->GetTimerManager().SetTimer(LifeTimeHandle, this, &ThisClass::ReturnToPool,LifeTime, false);
}

void UPoolableLifeTimeComponent::ReturnToPool()
{
	Debug::Print(TEXT("ReturnToPool"));
}
