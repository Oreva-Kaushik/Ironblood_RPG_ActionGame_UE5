// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "items/Items.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class NEWGAME_API ASoul : public AItems
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "Soul properties")
	int32 Souls;

	double DesiredZpoint;

	UPROPERTY(EditAnywhere)
	float DriftRate = -25.f;
	
public:
	FORCEINLINE int32 Getsouls() const { return Souls; }
	FORCEINLINE void SetSouls(int32 NumofSouls) { Souls = NumofSouls; }

};
