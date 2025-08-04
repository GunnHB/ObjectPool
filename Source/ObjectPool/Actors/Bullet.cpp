// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

ABullet::ABullet()
{
	bReplicates = true;
}

void ABullet::OnActivate()
{
	UE_LOG(LogTemp, Warning, TEXT("OnActivate"));
}

void ABullet::OnDeactivate()
{
	UE_LOG(LogTemp, Warning, TEXT("OnDeactivate"));
}
