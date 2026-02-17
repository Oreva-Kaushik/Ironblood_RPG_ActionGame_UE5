// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Items.h"
#include "newgame/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PickupInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/KKCharacter.h"

AItems::AItems()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect->SetupAttachment(GetRootComponent());

}

void AItems::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItems::OnSphereOverlap);

	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItems::OnSphereEndOverlap);

}

float AItems::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConst);
}

float AItems::TransformetCosin()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConst);
}

void AItems::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* Pickupinterface = Cast<IPickupInterface>(OtherActor);
	if (Pickupinterface) {
		Pickupinterface->SetOverlappingItem(this);
	}

	bool bTrapActivated = false;

	if (!bTrapActivated && IsTrap && OtherActor && Cast<AKKCharacter>(OtherActor))
	{
		bTrapActivated = true;

		GetWorld()->GetTimerManager().SetTimer(TrapDelayHandle, this, &AItems::TrapCharacter, Traptime, false);

	}

}

void AItems::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickupInterface* Pickupinterface = Cast<IPickupInterface>(OtherActor);
	if (Pickupinterface) {
		Pickupinterface->SetOverlappingItem(nullptr);
	}
}

void AItems::SpawnPickupsystem()
{
	if (PickupEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation()
		);
	}
}

void AItems::SpawnPickupsound()
{
	if (PickupSound) {
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			PickupSound,
			GetActorLocation()
		);
	}
}

void AItems::TrapCharacter()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sphere->SetCollisionResponseToAllChannels(ECR_Block);
}

void AItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	////Movment rate Unit is cm/s
	//float MovementRate = 50.f;
	//float RotationRate = 45.f;

	//// MovementRate * DeltaTime, (cm/s) * (s/frame) = cm/frame
	//AddActorWorldOffset(FVector(MovementRate * DeltaTime, 0.f, 0.f));
	//AddActorWorldRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));

	RunningTime += DeltaTime;

	//float DeltaZ = 
	//AddActorWorldOffset(FVector(0.f,0.f,DeltaZ));

	//DRAW_SPHERE_SingleFrame(GetActorLocation()); 
	//DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);

	//FVector AvgVector = Avg<FVector>(GetActorLocation(), FVector::ZeroVector);
	//DRAW_POINT_SingleFrame(AvgVector);
	if (Itemstate == EItemstate::EIS_Hovering) {
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}

}

