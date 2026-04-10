# Object Pooling Plugin

Reusable **Unreal Engine 5** plugin implementing the **Object Pooling** pattern as a lightweight `UActorComponent`.

Pre-spawns a configurable number of actors at startup, then recycles them via `AcquireObject` / `ReturnObject` instead of spawning and destroying at runtime — eliminating GC spikes and allocation overhead.

---

## Features

| Feature | Details |
|---|---|
| **Component-based** | Drop `UObjectPoolingComponent` onto any actor — no subsystems, no singletons. |
| **Blueprint & C++ friendly** | All public API is `BlueprintCallable`; delegates are `BlueprintAssignable`. |
| **IPoolableActor interface** | Optional `BlueprintNativeEvent` callbacks (`OnAcquiredFromPool`, `OnReturnedToPool`) for pooled actors. |
| **Delegates** | `OnPoolInitialized`, `OnPoolDeinitialized`, `OnAcquireObject`, `OnReturnObject` — bind from BP or C++. |
| **Parking Zone** | Inactive actors are hidden, tick-disabled, collision-disabled and teleported to a configurable off-screen location. |
| **Auto-init / manual init** | `bInitializeOnBeginPlay` flag or call `InitializePool()` whenever you need. Cleanup runs automatically on `EndPlay`. |

---

## Installation

### As a plugin in an existing project

1. Copy the `Plugins/ObjectPooling/` folder into your project's `Plugins/` directory.
2. Regenerate project files (right-click `.uproject` → *Generate Visual Studio project files*).
3. Add `"ObjectPooling"` to your module's `Build.cs` if you need C++ access:

```csharp
PublicDependencyModuleNames.Add("ObjectPooling");
```

4. Enable the plugin in **Edit → Plugins** (search *Object Pooling Plugin*) if it is not auto-enabled.

---

## Quick Start

### Blueprint

1. Add **ObjectPoolingComponent** to an actor (e.g. a weapon, spawner, manager).
2. In the component's **Details** panel set:
   - `ObjectClass` — the actor class to pool (e.g. `BP_Projectile`).
   - `PoolSize` — how many instances to pre-spawn (1–100).
   - `ParkingZoneLocation` — where inactive actors wait (default: `0, 0, -10000`).
   - `bInitializeOnBeginPlay` — whether the pool auto-initializes.
3. Call **AcquireObject(SpawnTransform)** to get an actor from the pool.
4. Call **ReturnObject(Actor)** to return it when done.

### C++

```cpp
// Acquire
FTransform SpawnTF = GetMuzzleTransform();
AActor* Projectile = PoolComponent->AcquireObject(SpawnTF);

// Return
PoolComponent->ReturnObject(Projectile);
```

### IPoolableActor Interface (optional)

Implement `IPoolableActor` on your pooled actor class to receive lifecycle callbacks:

| Callback | When |
|---|---|
| `OnAcquiredFromPool(SpawnTransform, OwningPool)` | Actor was just activated and placed at `SpawnTransform`. |
| `OnReturnedToPool()` | Actor was just deactivated and moved to the parking zone. |

In **Blueprint**: add the interface in *Class Settings → Interfaces*, then override the events.

In **C++**:

```cpp
UCLASS()
class AMyProjectile : public AActor, public IPoolableActor
{
    GENERATED_BODY()

    virtual void OnAcquiredFromPool_Implementation(const FTransform& SpawnTransform,
                                                    UObjectPoolingComponent* OwningPool) override;
    virtual void OnReturnedToPool_Implementation() override;
};
```

---

## API Reference

### UObjectPoolingComponent

#### Configuration (UPROPERTY)

| Property | Type | Default | Description |
|---|---|---|---|
| `bInitializeOnBeginPlay` | `bool` | `true` | Auto-init pool on `BeginPlay`. |
| `ObjectClass` | `TSubclassOf<AActor>` | `nullptr` | Actor class to pool. |
| `PoolSize` | `int32` | `1` | Number of pre-spawned actors (1–100). |
| `ParkingZoneLocation` | `FVector` | `(0, 0, -10000)` | Off-screen storage location. |

#### Functions (UFUNCTION)

| Function | Returns | Description |
|---|---|---|
| `InitializePool()` | `void` | Spawns actors, fills the inactive pool, broadcasts `OnPoolInitialized`. |
| `DeinitializePool()` | `void` | Destroys all actors, clears arrays, broadcasts `OnPoolDeinitialized`. |
| `AcquireObject(SpawnTransform)` | `AActor*` | Activates and returns an actor, or `nullptr` if pool is empty. |
| `ReturnObject(Actor)` | `void` | Deactivates actor, returns it to the inactive pool. |
| `GetInactiveObjectPool()` | `const TArray<AActor*>&` | Current inactive actors. |
| `GetActiveObjectPool()` | `const TArray<AActor*>&` | Current active actors. |

#### Delegates

| Delegate | Params | Fires when |
|---|---|---|
| `OnPoolInitialized` | `TArray<AActor*>` pool | Pool fully initialized. |
| `OnPoolDeinitialized` | — | Pool deinitialized. |
| `OnAcquireObject` | `AActor*`, `FTransform` | Actor acquired from pool. |
| `OnReturnObject` | — | Actor returned to pool. |

---

## Project Structure

```
Plugins/ObjectPooling/
├── ObjectPooling.uplugin
└── Source/ObjectPooling/
    ├── ObjectPooling.Build.cs
    ├── Public/
    │   ├── ObjectPooling.h              // Module & log category
    │   ├── ObjectPoolingComponent.h     // Main component
    │   └── ObjectPoolingInterface.h     // IPoolableActor interface
    └── Private/
        ├── ObjectPooling.cpp            // Module implementation
        └── ObjectPoolingComponent.cpp   // Component implementation
```

---

## Compatibility

- **Engine**: Unreal Engine 5 (developed on 5.5+)
- **Platforms**: All platforms supported by UE5 (no platform-specific code)
- **Module type**: Runtime

---

## License

Copyright © Jakub Balcerzak. All rights reserved.

