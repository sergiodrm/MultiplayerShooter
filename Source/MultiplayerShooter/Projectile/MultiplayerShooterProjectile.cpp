#include "MultiplayerShooterProjectile.h"

#include "MultiplayerShooter/MultiplayerShooter.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerShooter/MultiplayerShooterGameMode.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayerShooterProjectile::AMultiplayerShooterProjectile()
{
    // Components
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->InitSphereRadius(37.5f);
    SphereComponent->SetCollisionProfileName(FMultiplayerShooterGlobals::ProjectileCollisionChannelName);
    RootComponent = SphereComponent;
    if (GetLocalRole() == ROLE_Authority)
    {
        SphereComponent->OnComponentHit.AddDynamic(this, &AMultiplayerShooterProjectile::OnProjectileImpact);
    }

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);

    static const ConstructorHelpers::FObjectFinder<UStaticMesh> defaultMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
    if (defaultMesh.Succeeded())
    {
        StaticMeshComponent->SetStaticMesh(defaultMesh.Object);
        StaticMeshComponent->SetRelativeLocation({0.f, 0.f, -37.5f});
        StaticMeshComponent->SetRelativeScale3D(FVector {0.75f});
        if (ProjectileMaterial)
        {
            UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(ProjectileMaterial, this);
            StaticMeshComponent->SetMaterial(0, material);
        }
    }

    static const ConstructorHelpers::FObjectFinder<UParticleSystem> defaultParticleSystem(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
    if (defaultParticleSystem.Succeeded())
    {
        ExplosionEffect = defaultParticleSystem.Object;
    }

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
    ProjectileMovementComponent->InitialSpeed = 1500.f;
    ProjectileMovementComponent->MaxSpeed = 1500.f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->ProjectileGravityScale = 0.f;

    // Default values
    DamageType = UDamageType::StaticClass();
    Damage = 10.f;
    bReplicates = true;
}


void AMultiplayerShooterProjectile::Destroyed()
{
    Super::Destroyed();

    UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, GetActorLocation(),
                                             FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
}

void AMultiplayerShooterProjectile::SetProjectileColor(FColor color)
{
    UMaterialInstanceDynamic* material = Cast<UMaterialInstanceDynamic>(StaticMeshComponent->GetMaterial(0));
    if (material)
    {
        material->SetVectorParameterValue(FMultiplayerShooterGlobals::ProjectileColorParameterName, color);
    }
}

void AMultiplayerShooterProjectile::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor)
    {
        UGameplayStatics::ApplyPointDamage(OtherActor, Damage, NormalImpulse, Hit, GetInstigator()->Controller, this, DamageType);
    }

    Destroy();
}
