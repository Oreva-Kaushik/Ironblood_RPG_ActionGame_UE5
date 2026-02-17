// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GAnimInstance.h"
#include "Characters/KKCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UGAnimInstance::NativeInitializeAnimation() {

	Super::NativeInitializeAnimation();

	KKCharacter = Cast<AKKCharacter>(TryGetPawnOwner());
	
	if (KKCharacter) {
		KKCharacterMovement = KKCharacter->GetCharacterMovement();
	}

}

void UGAnimInstance::NativeUpdateAnimation(float Deltatime) {

	Super::NativeUpdateAnimation(Deltatime);

	if (KKCharacterMovement) {
		
		Groundspeed = UKismetMathLibrary::VSizeXY(KKCharacterMovement->Velocity);
		IsFalling = KKCharacterMovement->IsFalling();
		Charstate = KKCharacter->GetCharstate();
		Actionstate = KKCharacter->GetActionstate();
		Diedstate = KKCharacter->GetDiedstate();
	}

}