// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerShooterCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MultiplayerShooter/MultiplayerShooter.h"
#include "MultiplayerShooter/Game/MultiplayerShooterGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerShooter/Projectile/MultiplayerShooterProjectile.h"

//////////////////////////////////////////////////////////////////////////
// AMultiplayerShooterCharacter

AMultiplayerShooterCharacter::AMultiplayerShooterCharacter()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // set our turn rates for input
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
    // are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

    ProjectileClass = AMultiplayerShooterProjectile::StaticClass();
    FireRate = 0.25f;
    IsFiringWeapon = false;
}

void AMultiplayerShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMultiplayerShooterCharacter, CurrentHealth);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMultiplayerShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Set up gameplay key bindings
    check(PlayerInputComponent);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAction(FMultiplayerShooterGlobals::FireInputActionName, IE_Pressed, this, &AMultiplayerShooterCharacter::StartFire);

    PlayerInputComponent->BindAxis("MoveForward", this, &AMultiplayerShooterCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMultiplayerShooterCharacter::MoveRight);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &AMultiplayerShooterCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AMultiplayerShooterCharacter::LookUpAtRate);
}

void AMultiplayerShooterCharacter::OnRep_CurrentHealth()
{
    OnHealthUpdated();
}

void AMultiplayerShooterCharacter::OnHealthUpdated()
{
    // Client side
    if (IsLocallyControlled())
    {
        const FString healthMessage = FString::Printf(TEXT("Current Health: %f"), CurrentHealth);
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue, healthMessage);
        if (CurrentHealth <= 0)
        {
            const FString deathMessage = FString::Printf(TEXT("You died."));
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, deathMessage);
        }
    }

    // Server side
    if (GetLocalRole() == ROLE_Authority)
    {
        const FString healthMessage = FString::Printf(TEXT("%s has %f health points."), *GetFName().ToString(), CurrentHealth);
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue, healthMessage);
    }
}

void AMultiplayerShooterCharacter::SetCurrentHealth(float healthValue)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
        OnHealthUpdated();
    }
}

float AMultiplayerShooterCharacter::TakeDamage(float damageTaken, const FDamageEvent& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
    const float remainingHealth = CurrentHealth - damageTaken;
    SetCurrentHealth(remainingHealth);
    return remainingHealth;
}

void AMultiplayerShooterCharacter::StartFire()
{
    if (!IsFiringWeapon)
    {
        IsFiringWeapon = true;
        GetWorld()->GetTimerManager().SetTimer(FiringTimer, this, &AMultiplayerShooterCharacter::StopFire, FireRate, false);
        HandleFire();
    }
}

void AMultiplayerShooterCharacter::StopFire()
{
    IsFiringWeapon = false;
}

void AMultiplayerShooterCharacter::SetTeam(EMultiplayerShooterTeam team)
{
    if (GetLocalRole() == ROLE_Authority && team != GetTeam())
    {
        Team = team;

        // TODO change character color
    }
}

void AMultiplayerShooterCharacter::HandleFire_Implementation()
{
    const FVector spawnLocation = GetActorLocation() + (GetControlRotation().Vector() * 100.f) + (GetActorUpVector() * 50.f);
    const FRotator spawnRotation = GetControlRotation();

    FActorSpawnParameters spawnParameters;
    spawnParameters.Instigator = GetInstigator();
    spawnParameters.Owner = this;
    AMultiplayerShooterProjectile* projectile = GetWorld()->SpawnActor<AMultiplayerShooterProjectile>(spawnLocation, spawnRotation, spawnParameters);
    if (AMultiplayerShooterGameMode* gameMode = Cast<AMultiplayerShooterGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
    {
        projectile->SetProjectileColor(gameMode->GetTeamColor(GetTeam()));
    }
}


void AMultiplayerShooterCharacter::TurnAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMultiplayerShooterCharacter::LookUpAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMultiplayerShooterCharacter::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void AMultiplayerShooterCharacter::MoveRight(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get right vector 
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}
