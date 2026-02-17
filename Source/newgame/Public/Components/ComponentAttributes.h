// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComponentAttributes.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWGAME_API UComponentAttributes : public UActorComponent
{
	GENERATED_BODY()

public:	
	UComponentAttributes();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RegenStamina(float DeltaTime);
	void RegenHealth(float DeltaTime);

protected:
	virtual void BeginPlay() override;

private:
	//CurrentHealth
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Soul;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Dodgecost = 15.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Abillitycost = 50.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Staminagainrate = 8.5f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Healthgainrate = 3.5f;

public:

	void ReceiveDamage(float Damage);
	void UseStamina(float Staminause);
	float GetHealthpercent();
	float GetStaminapercent();
	bool IsAlive();
	
	void AddGolds(int32 NumGolds);
	void AddSouls(int32 NumSouls);

	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSoul() const { return Soul; }
	FORCEINLINE float GetDodgecost() const { return Dodgecost; }
	FORCEINLINE float GetAbillitycost() const { return Abillitycost; }
	FORCEINLINE float GetStamina() const { return Stamina; }
};
