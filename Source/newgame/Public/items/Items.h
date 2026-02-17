// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items.generated.h"

class USphereComponent;
class UNiagaraComponent;

enum class EItemstate : uint8 
{
	EIS_Hovering,
	EIS_Equipped,
};

UCLASS()
class NEWGAME_API AItems : public AActor
{
	GENERATED_BODY()
	
public:	
	AItems();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine parameter")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine parameter")
	float TimeConst = 5.f;

	UFUNCTION(BlueprintPure)
	float TransformedSin();

	UFUNCTION(BlueprintPure)
	float TransformetCosin();

	template<typename T>
	T Avg(T First, T Second);

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void SpawnPickupsystem();
	virtual void SpawnPickupsound();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ItemMesh;

	EItemstate Itemstate = EItemstate::EIS_Hovering;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* ItemEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;

	UPROPERTY(EditAnywhere)
	bool IsTrap = false;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true") )
	float RunningTime;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickupEffect;

	UPROPERTY(EditAnywhere)
	float Traptime = 1.5f;

	FTimerHandle TrapDelayHandle;

	UFUNCTION()
	void TrapCharacter();

};

template<typename T>
inline T AItems::Avg(T First, T Second)
{
	return (First + Second) / 2;
}