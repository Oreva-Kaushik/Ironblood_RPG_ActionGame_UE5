// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyInstance.h"
#include "Enemies/Enemy.h"

void UEnemyInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	EnemyRef = Cast<AEnemy>(TryGetPawnOwner());
}

void UEnemyInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!EnemyRef)
	{
		EnemyRef = Cast<AEnemy>(TryGetPawnOwner());
	}

	if (!EnemyRef) return;

	if (EnemyRef) {
		bHasWokenUp = EnemyRef->bHasWokenUp;
		bIsBossEnemy = EnemyRef->bIsBossEnemy;
	}
}

