// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LockableTarget.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULockableTarget : public UInterface
{
	GENERATED_BODY()
};

class NEWGAME_API ILockableTarget
{
	GENERATED_BODY()

public:
	virtual FVector GetTargetLockLocation() const = 0;

};
