// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolSubsystem.h"

UObjectPoolSubsystem::UObjectPoolSubsystem()
{
	ObjectPoolDataTable = TSoftObjectPtr<UDataTable>(FSoftObjectPath(""));
}

void UObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (GetWorld()->IsGameWorld() == false)
		return;

	if (GetWorld()->GetNetMode() == NM_Client)
		return;

	InitPool();
}

void UObjectPoolSubsystem::InitPool()
{
	if (ObjectPoolDataTable.IsNull())
		return;

	UDataTable* LoadedDataTable = ObjectPoolDataTable.LoadSynchronous();
	if (IsValid(LoadedDataTable))
		AsyncLoadObject();
}

void UObjectPoolSubsystem::AsyncLoadObject()
{
	
}

void UObjectPoolSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UObjectPoolSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
}

AActor* UObjectPoolSubsystem::GetObjectFromPool(const FGameplayTag InGameplayTag, AActor* Owner)
{
	return nullptr;
}
