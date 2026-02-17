// Fill out your copyright notice in the Description page of Project Settings.


#include "Patrolpoints/APatrolManager.h"

AAPatrolManager::AAPatrolManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AAPatrolManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAPatrolManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

