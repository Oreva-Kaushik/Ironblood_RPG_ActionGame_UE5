// Fill out your copyright notice in the Description page of Project Settings.


#include "Patrolpoints/FakePatrolpawn.h"

AFakePatrolpawn::AFakePatrolpawn()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorHiddenInGame(false);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

}

void AFakePatrolpawn::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(2.f);

}

void AFakePatrolpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFakePatrolpawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

