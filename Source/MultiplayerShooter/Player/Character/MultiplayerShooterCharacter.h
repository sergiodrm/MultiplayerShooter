// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "MultiplayerShooter/Game/MultiplayerShooterTeam.h"
#include "GameFramework/Character.h"
#include "MultiplayerShooterCharacter.generated.h"

UCLASS(config=Game)
class AMultiplayerShooterCharacter : public ACharacter
{
    GENERATED_BODY()


public:
    AMultiplayerShooterCharacter();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    /** Called for forwards/backward input */
    void MoveForward(float Value);

    /** Called for side to side input */
    void MoveRight(float Value);

    /**
     * Called via input to turn at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void TurnAtRate(float Rate);

    /**
     * Called via input to turn look up/down at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void LookUpAtRate(float Rate);

protected:
    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    // End of APawn interface

    UFUNCTION()
    void OnRep_CurrentHealth();

    void OnHealthUpdated();

public:
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    /** Returns FollowCamera subobject **/
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    UFUNCTION(BlueprintPure, Category = "Health")
    FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

    UFUNCTION(BlueprintPure, Category = "Health")
    FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetCurrentHealth(float healthValue);
    UFUNCTION(BlueprintCallable, Category = "Health")
    virtual float TakeDamage(float damageTaken, const struct FDamageEvent& damageEvent, AController* eventInstigator, AActor* damageCauser) override;

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void StartFire();
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void StopFire();
    UFUNCTION(Server, Reliable)
    void HandleFire();

    FORCEINLINE EMultiplayerShooterTeam GetTeam() const { return Team; }
    void SetTeam(EMultiplayerShooterTeam team);

private:
    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Health", meta = (ClampMin = "0.0"))
    float MaxHealth {100.f};
    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_CurrentHealth, Category = "Health")
    float CurrentHealth;

    UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
    TSubclassOf<class AMultiplayerShooterProjectile> ProjectileClass;
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
    float FireRate;

    bool IsFiringWeapon {false};

    FTimerHandle FiringTimer;

    UPROPERTY(VisibleAnywhere, Category = "Gameplay")
    EMultiplayerShooterTeam Team;

public:
    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float BaseLookUpRate;
};
