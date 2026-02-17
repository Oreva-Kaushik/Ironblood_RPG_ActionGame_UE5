// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Charactertype.h"
#include "GAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NEWGAME_API UGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float Deltatime) override; 

	UPROPERTY(BlueprintReadOnly)
	class AKKCharacter* KKCharacter;
	 
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* KKCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Groundspeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	ECharstate Charstate;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	EActionstate Actionstate;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TEnumAsByte<EDiedstate> Diedstate;

};
