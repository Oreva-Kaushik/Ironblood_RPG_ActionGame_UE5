// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Treasure/Treasure.h"
#include "Interfaces/PickupInterface.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	IPickupInterface* Pickupinterface = Cast<IPickupInterface>(OtherActor);
	if (Pickupinterface) {
		Pickupinterface->AddGolds(this);
		SpawnPickupsound();
		Destroy();
	}
}
