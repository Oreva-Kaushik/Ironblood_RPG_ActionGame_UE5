// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ComponentAttributes.h"


UComponentAttributes::UComponentAttributes()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UComponentAttributes::BeginPlay()
{
	Super::BeginPlay();

	
}

void UComponentAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UComponentAttributes::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + Staminagainrate * DeltaTime, 0.f, MaxStamina);

}

void UComponentAttributes::RegenHealth(float DeltaTime)
{
	Health = FMath::Clamp(Health + Healthgainrate * DeltaTime, 0.f, MaxHealth);
}

void UComponentAttributes::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

void UComponentAttributes::UseStamina(float Staminause)
{
	Stamina = FMath::Clamp(Stamina - Staminause, 0.f, MaxStamina);
}

float UComponentAttributes::GetHealthpercent()
{
	return Health/MaxHealth ;
}

float UComponentAttributes::GetStaminapercent()
{
	return Stamina/ MaxStamina;
}

bool UComponentAttributes::IsAlive()
{
	return Health > 0.f;
}

void UComponentAttributes::AddGolds(int32 NumGolds)
{
	Gold += NumGolds;
}

void UComponentAttributes::AddSouls(int32 NumSouls)
{
	Soul += NumSouls;
}
