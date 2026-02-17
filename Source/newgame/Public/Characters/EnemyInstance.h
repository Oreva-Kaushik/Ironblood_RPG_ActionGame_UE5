// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyInstance.generated.h"

/**
 * 
 */
UCLASS()
class NEWGAME_API UEnemyInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "References")
	class AEnemy* EnemyRef;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bHasWokenUp;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsBossEnemy;

};
