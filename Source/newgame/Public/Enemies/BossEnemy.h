// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Enemy.h"
#include "Interfaces/LockableTarget.h"
#include "BossEnemy.generated.h"

/**
 * 
 */
UCLASS()
class NEWGAME_API ABossEnemy : public AEnemy
{
	GENERATED_BODY()
	
public:
	ABossEnemy();

	//virtual FVector GetTargetLockLocation() const override;
};
