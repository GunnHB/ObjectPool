// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPool/Interfaces/PoolableInterface.h"
#include "Bullet.generated.h"

UCLASS()
class OBJECTPOOL_API ABullet : public AActor, public IPoolableInterface
{
	GENERATED_BODY()

public:
	ABullet();

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;
};
