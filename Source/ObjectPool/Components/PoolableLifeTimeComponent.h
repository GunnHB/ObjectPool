// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoolableLifeTimeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OBJECTPOOL_API UPoolableLifeTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void ActivateLifeTimer();

private:
	FTimerHandle LifeTimeHandle;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	float LifeTime = 0.f;

	void ReturnToPool();
};
