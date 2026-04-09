// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPoolInitialized, const TArray<AActor*>&, ObjectPool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPoolDeinitialized);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OBJECTPOOLING_API UObjectPoolingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UObjectPoolingComponent();
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/* Delegates */
	UPROPERTY(BlueprintAssignable, Category = "Object Pooling")
	FOnPoolInitialized OnPoolInitialized;
	
	UPROPERTY(BlueprintAssignable, Category = "Object Pooling")
	FOnPoolDeinitialized OnPoolDeinitialized;
	
	/* Configuration */
	
	// Whether to automatically initialize pool on BeginPlay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pooling")
	bool bInitializeOnBeginPlay = true;
	
	// Actor class to spawn in the pool
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pooling")
	TSubclassOf<AActor> ObjectClass = nullptr;
	
	// Number of actors to pre-spawn in the pool
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pooling", meta = (ClampMin = 1, ClampMax = 100))
	int32 PoolSize = 1;
	
	// Location where inactive actors will be waiting for acquisition
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pooling")
	FVector ParkingZoneLocation = FVector(0, 0, -10000);

	// Spawns actors and fills the pool
	UFUNCTION(BlueprintCallable, Category = "Object Pooling")
	void InitializePool();
	
	// Destroys all pooled actors and clears the pool
	UFUNCTION(BlueprintCallable, Category = "Object Pooling")
	void DeinitializePool();
	
private:
	UPROPERTY()
	TArray<AActor*> ObjectPool;
	
	bool bInitialized = false;
	FTransform ParkingZoneTransform;
	
	void ActivateActor(AActor* Actor, const FTransform& Transform);
	void DeactivateActor(AActor* Actor) const;
	
	/* Getters and Setters */
public:
	// Get array of objects in pool
	UFUNCTION(BlueprintCallable, Category = "Object Pooling")
	const TArray<AActor*>& GetObjectPool() const { return ObjectPool; }
};

