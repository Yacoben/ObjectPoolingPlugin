// Copyright Epic Games, Inc. All Rights Reserved.

#include "ObjectPoolingComponent.h"
#include "ObjectPoolingInterface.h"
#include "ObjectPooling.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "TimerManager.h"

UObjectPoolingComponent::UObjectPoolingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObjectPoolingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (bInitializeOnBeginPlay)
	{
		InitializePool();
	}
}

void UObjectPoolingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DeinitializePool();
	
	Super::EndPlay(EndPlayReason);
}

void UObjectPoolingComponent::InitializePool()
{
	if (bInitialized) return;
	if (!IsValid(GetWorld())) return;
	
	for (int32 i = 0 ; i < PoolSize; i++)
	{
		if (AActor* ActorDeferred = GetWorld()->SpawnActorDeferred<AActor>(ObjectClass, FTransform::Identity, GetOwner()))
		{
			ActorDeferred->SetActorHiddenInGame(true);
			ActorDeferred->SetActorTickEnabled(false);
			ActorDeferred->SetActorEnableCollision(false);
			ActorDeferred->FinishSpawning(FTransform::Identity);
			ObjectPool.AddUnique(ActorDeferred);
		}
	}
	
	bInitialized = true;
}

void UObjectPoolingComponent::DeinitializePool()
{
	if (!IsValid(GetWorld())) return;
	
	for (int32 i = 0 ; i < ObjectPool.Num(); i++)
	{
		if (ObjectPool[i])
		{
			ObjectPool[i]->Destroy();
		}
	}
	ObjectPool.Empty();
}

