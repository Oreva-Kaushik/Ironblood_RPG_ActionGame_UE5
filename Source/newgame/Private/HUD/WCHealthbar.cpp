// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WCHealthbar.h"
#include "HUD/Healthbar.h"
#include "Components/ProgressBar.h"

void UWCHealthbar::SetHealthPercent(float Percent)
{
	if (Healthbarwidget == nullptr) {

		Healthbarwidget = Cast<UHealthbar>(GetUserWidgetObject());
	}
	if (Healthbarwidget && Healthbarwidget->Healthbar) {
		Healthbarwidget->Healthbar->SetPercent(Percent);
	}
}
