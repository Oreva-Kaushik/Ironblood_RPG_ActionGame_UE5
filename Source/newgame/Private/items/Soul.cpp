// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Soul.h"
#include "Interfaces/PickupInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void ASoul::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	const double Location = GetActorLocation().Z;
	if (Location > DesiredZpoint) {

		const FVector Deltalocation = FVector(0.f, 0.f, DriftRate * DeltaTime);
		AddActorWorldOffset(Deltalocation);
	}
}

void ASoul::BeginPlay() {
	Super::BeginPlay();

	const FVector Startlocation = GetActorLocation();
	const FVector Endlocation = Startlocation - FVector(0.f,0.f,2000.f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	TArray<AActor* > ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult Hitresult;

	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Startlocation,
		Endlocation,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		Hitresult,
		true
	);
	DesiredZpoint = Hitresult.ImpactPoint.Z + 115.f;
}

void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	IPickupInterface* Pickupinterface = Cast<IPickupInterface>(OtherActor);
	if (Pickupinterface) {
		Pickupinterface->AddSouls(this);
		SpawnPickupsystem();
		SpawnPickupsound();
		Destroy();
	}
}
