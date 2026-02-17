// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h" 
#include "Characters/Charactertype.h"
#include "Enemy.generated.h"

class AWeapon;

UCLASS()
class NEWGAME_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	/* <AActor> */
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Destroyed() override;
	/* </AActor> */

	/* <IHitinterface> */
	virtual void GetHit_Implementation(const FVector& Impactpoint, AActor* Hitter) override;
	/* </IHitinterface> */

	//Current patrol target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* Patroltarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> Patroltargets;

	/* Boss Enemy Component */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Detection")
	class USphereComponent* DetectionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Detection")
	USphereComponent* OuterDetectionSphere;

	// Is this enemy a boss?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Type")
	bool bIsBossEnemy = false;

	// Has the boss already woken up?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bHasWokenUp = false;

	bool bHasEnteredOuter = false;

	// Wake-up animation
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* WakeUpMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* PoseMontage;

	bool bIsFrozen = false;

	FTimerHandle FreezeTimerHandle;

	FTimerHandle WeaponSpawntimer;

	UFUNCTION(BlueprintCallable)
	void FreezeEnemy(float Duration);

	void UnFreezeEnemy();

protected:

	virtual void BeginPlay() override;
	void SpawnFakePawn();

	/* <ABaseCharacter> */
	virtual void Die() override;
	void SpawnSouls();
	void SpawnWeapon();
	virtual void Attack() override;
	virtual bool CcanAttack() override;
	virtual void Attackend() override;
	virtual void Handledamage(float DamageAmount) override;
	/* </ABaseCharacter> */

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EEnemystate Enemystate = EEnemystate::EES_Patrolling;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponSpawnclass;

private:

	// AI Behavior //
	void IntializeEnemy();
	void CheckpatrolTarget();
	void CheckcombatTarget();
	void PatroltimerFinished();
	void HideHealthbar();

	UFUNCTION(BlueprintCallable)
	void ShowHealthbar();

	void Loseinterest();
	void StartPatrolling();
	void ChasingTarget();
	bool IsOutsideCombatradius();
	bool IsOutsideAttackRadius();
	bool InsideAttackradius();
	bool IsChasing();
	bool IsAttacking();

	UFUNCTION(BlueprintCallable)
	bool IsDead();

	bool IsEngaged();
	void ClearPatroltimer();
	void Startattacktimer();
	void Clearattacktimer();
	bool InTargetRange(AActor* Target, double Radius);
	void MovetoTarget(AActor* Target);
	AActor* ChoosepatrolTarget();
	void SpawnDefaultWeapon();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // Callback for OnPawnSeen

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensingCp;

	UPROPERTY(VisibleAnywhere)
	class UWCHealthbar* WidgetHealthbar;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = Radius);
	double CombatRadius = 1250.f;

	UPROPERTY(EditAnywhere, Category = Radius);
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = Radius);
	double AcceptanceRadius = 50.f;

	UPROPERTY(EditAnywhere, Category = Radius);
	bool bQuickResponsivetoAttckradius = false;
	 
	UPROPERTY()
	class AAIController* EnemyAIController;

	UPROPERTY(EditAnywhere, Category = Radius);
	double PatrolRadius = 300.f;

	FTimerHandle Patroltimer;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	float Waitmin = 3.f;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	float Waitmax = 8.f;

	UPROPERTY(EditAnyWhere, Category = Combat)
	float Patrollingspeed = 150.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat);
	float Attackmin = 0.5f;

	UPROPERTY(EditAnywhere, Category = Combat);
	float Attackmax = 0.8f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float chasingspeed = 300.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathlifeSpan = 15.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class ASoul> Soulclass;


	/* Boss Enemy Component */

	UFUNCTION()
	void OnPlayerEnterRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerOuterRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	void PlayWakeUpMontage();
	void PlayPoseMontage();
	void EnableNormalBehavior();

	FTimerHandle TimerHandle_EnableNormalBehavior;

};