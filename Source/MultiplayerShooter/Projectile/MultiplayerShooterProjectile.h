#pragma once

#include "CoreMinimal.h"
#include "MultiplayerShooterProjectile.generated.h"

UCLASS(Config=Game)
class AMultiplayerShooterProjectile : public AActor
{
    GENERATED_BODY()
public:
    AMultiplayerShooterProjectile();

    virtual void Destroyed() override;

    void SetProjectileColor(FColor color);

protected:
    UFUNCTION(Category = "Projectile")
    void OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class USphereComponent* SphereComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* StaticMeshComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class UMaterial* ProjectileMaterial;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
    class UParticleSystem* ExplosionEffect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UDamageType> DamageType;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    float Damage;
};
