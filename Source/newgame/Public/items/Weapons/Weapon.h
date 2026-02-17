// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "items/Items.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;

/**
 * 
 */

UCLASS()
class NEWGAME_API AWeapon : public AItems
{
	GENERATED_BODY()

public:
	AWeapon();

	void Equip(USceneComponent* Inparent, FName Insocketname, AActor* NewOwner, APawn* NewInstigator);
	void DeactivateEmbers();
	void DisableSphereCollision();
	void PlayequipSound();
	void DetachItem();

	void Attachmeshtosocket(USceneComponent* Inparent, const FName& Insocketname);
	void Dettachmeshtosocket();

	TArray<AActor*> IgnoreActors;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxoverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool ActorisSametype(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& BoxHit);

	UFUNCTION(BlueprintImplementableEvent)
	void Createfields(const FVector& Fieldlocation);

private:

	void BOXtrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = false;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* Equipsound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* Weaponbox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Boxtracestart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Boxtraceend;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20;

public:
	FORCEINLINE UBoxComponent* GetWeaponbox() const { return Weaponbox; }
	
};
