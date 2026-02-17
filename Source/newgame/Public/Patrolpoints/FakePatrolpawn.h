// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FakePatrolpawn.generated.h"

UCLASS()
class NEWGAME_API AFakePatrolpawn : public APawn
{
	GENERATED_BODY()

public:
	AFakePatrolpawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
