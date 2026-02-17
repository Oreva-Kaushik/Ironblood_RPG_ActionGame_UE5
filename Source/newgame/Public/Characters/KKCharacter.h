// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Charactertype.h"
#include "Interfaces/PickupInterface.h"
#include "KKCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AItems;
class ATreasure;
class ASoul;
class UAnimMontage;
class UCharoverlay;


UCLASS()
class NEWGAME_API AKKCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	AKKCharacter();
	virtual void Tick(float DeltaTime) override; 
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit_Implementation(const FVector& Impactpoint, AActor* Hitter) override;
	virtual void SetOverlappingItem(AItems* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGolds(ATreasure* Treasure) override;

protected:
	virtual void BeginPlay() override;

	/* Callback to Input keys*/
	void Moveforward(float Value); 
	void Moveside(float Value);
	void Turn(float Value);
	void Lookup(float Value);
	void EKeypress();
	void QKeypress();
	void Item1();
	void Item2();
	void Dodge();
	void Targetlock();

	UFUNCTION(BlueprintCallable)
	void Abillity();

	void AbilityDamage();

	virtual void Attack() override;

	/* Combat */
	virtual void Attackend() override;
	virtual void Dodgeend() override;
	virtual bool CcanAttack() override;
	bool CcanDisarm();
	bool Ccanarm();
	void PlayEquipmontage(const FName Sectionname);
	virtual void Die() override;
	void HandleDeath();
	void RestartGame();

	bool HasEnoughStamina();

	UFUNCTION(BlueprintCallable)
	bool HasEnoughAbilityStamina();

	UFUNCTION(BlueprintCallable)
	bool IsOccupied();

	UFUNCTION(BlueprintCallable)
	void Itemtakingend();

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void HitReactend();

	FTimerHandle RestartTimerHandle;

	/* Character Components */

	UPROPERTY(EditAnywhere)
	USpringArmComponent* CameraBone;

	UPROPERTY(EditAnywhere)
	UCameraComponent* ViewCamera;

	//Lookon

	UFUNCTION(BlueprintCallable)
	void DodgeZend();

	UPROPERTY()
	AActor* LockonActor = nullptr;

	UPROPERTY()
	bool bIsLockedOn = false;

	UPROPERTY(EditAnywhere, Category = "Lock-On")
	float LockRange = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Lock-On")
	float LockRadius = 700.f;

	UPROPERTY(EditAnywhere, Category = "Lock-On")
	FVector LockonSocketOffset = FVector(0.f, 0.f, 80.f);


	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<AActor> EnemyClass;

	FTimerHandle AbilityDelayHandle;

	/*
	bool bIsAbilityActive = false;

	UFUNCTION(BlueprintCallable)
	void AbilityActiveEnd();

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<AActor> AOEEffectClass;

	UFUNCTION()
	void SpawnlightningStrike();

	*/

private:

	bool IsUnoccupied();
	void ImplementOverlay();
	void SetHUDHealth();

	ECharstate Charstate = ECharstate::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionstate Actionstate = EActionstate::EAS_Unoccupied;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EItemtaking Itemtakingstate = EItemtaking::EIT_Nottaking;

	/* Character Components */

	UPROPERTY(VisibleInstanceOnly)
	AItems* OverLappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montages);
	UAnimMontage* EquipMontage;

	UPROPERTY()
	UCharoverlay* GetOverlay;

	/*
	UPROPERTY(EditDefaultsOnly, Category = Montages);
	UAnimMontage* Abilitymontage;
	*/

public:
	FORCEINLINE ECharstate GetCharstate() const { return Charstate; }
	FORCEINLINE EActionstate GetActionstate() const { return Actionstate; }

};
