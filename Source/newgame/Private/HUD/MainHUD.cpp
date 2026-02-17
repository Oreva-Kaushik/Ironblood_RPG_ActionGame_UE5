// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainHUD.h"
#include "HUD/Charoverlay.h"

void AMainHUD::BeginPlay() {
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World) {
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && Charoverlay) {
			Greyoverlay = CreateWidget<UCharoverlay>(Controller, Charoverlay);
			Greyoverlay->AddToViewport();
		}
	}

	
}