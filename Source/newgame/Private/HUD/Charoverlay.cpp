// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Charoverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCharoverlay::SetHealthbarPercent(float Percent)
{
	if (HealthProgressbar) {
		HealthProgressbar->SetPercent(Percent);
	 }
}

void UCharoverlay::SetStaminabarPercent(float Percent)
{
	if (StaminaProgressBar) {
		StaminaProgressBar->SetPercent(Percent);
	}
}

void UCharoverlay::SetGold(int32 Gold)
{
	if (Goldcount) {

		const FText GoldText = FText::FromString(FString::Printf(TEXT("%d"), Gold));
		Goldcount->SetText(GoldText);
	}
}

void UCharoverlay::SetSoul(int32 Soul)
{
	if (Soulcount) {

		const FText SoulText = FText::FromString(FString::Printf(TEXT("%d"), Soul));
		Soulcount->SetText(SoulText);
	}
}
