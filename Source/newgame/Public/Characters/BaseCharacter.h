// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Charactertype.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAnimMontage;

UCLASS()
class NEWGAME_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetHit_Implementation(const FVector& Impactpoint, AActor* Hitter) override;

	virtual void Attack();
	virtual void Die();
	void DirectionalHitreact(const FVector& Impactpoint);
	void PlayHitsound(const FVector& Impactpoint);
	void SpawnHitparticles(const FVector& Impactpoint);
	virtual void Handledamage(float DamageAmount);
	void DisableCapsule();	
	virtual bool CcanAttack();
	bool IsAlive();
	void DisablemeshCollision();

	void PlatHitreactmontage(const FName Sectionname);

	virtual int32 PlayAttackmontage();
	virtual int32 PlayDeathmontage();
	virtual void PlayDodgeMontage();
	void StopAttackMontage();

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarptarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarptarget();

	UFUNCTION(BlueprintCallable)
	virtual void Attackend();

	UFUNCTION(BlueprintCallable)
	virtual void Dodgeend();

	UFUNCTION(BlueprintCallable)
	void SetweaponcollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* Equippedweapon;

	UPROPERTY(VisibleAnywhere)
	class UComponentAttributes* Attributes;

	UPROPERTY(BlueprintReadOnly, Category = Combat);
	AActor* Combattarget;

	UPROPERTY(EditAnywhere, Category = Combat);
	double WarpTargetdistance = 75.f;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDiedstate> Diedstate;

	void PlaymontageSection(UAnimMontage* Montage, const FName& SectionName);

private:
		
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	UPROPERTY(EditAnywhere, Category = Combat);
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Combat);
	UParticleSystem* Hitparticles;

	UPROPERTY(EditDefaultsOnly, Category = Montages);
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages);
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages);
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages);
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Montages)
	TArray<FName> DeathMontageSections;
	
public:

	FORCEINLINE TEnumAsByte<EDiedstate> GetDiedstate() const { return Diedstate; }

};
