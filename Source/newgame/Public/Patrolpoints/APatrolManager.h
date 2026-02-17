// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APatrolManager.generated.h"

UCLASS()
class NEWGAME_API AAPatrolManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPatrolManager();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

};
