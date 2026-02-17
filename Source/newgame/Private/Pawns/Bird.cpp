// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


ABird::ABird()
{

	PrimaryActorTick.bCanEverTick = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(RootComponent);

	CameraBone = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBone->SetupAttachment(RootComponent);
	CameraBone->TargetArmLength = 220.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBone);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

void ABird::BeginPlay()
{
	Super::BeginPlay();

}

void ABird::Moveforward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f))
	{
		FVector forward = GetActorForwardVector();
		AddMovementInput(forward, Value);
	}
}

void ABird::Turn(float Value)
{

	AddControllerYawInput(Value);

}

void ABird::Lookup(float Value)
{
	AddControllerPitchInput(Value);
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("Moveforward"), this, &ABird::Moveforward);

	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABird::Turn);

	PlayerInputComponent->BindAxis(FName("Lookup"), this, &ABird::Lookup);

}

