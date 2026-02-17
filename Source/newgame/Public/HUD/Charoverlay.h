// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Charoverlay.generated.h"

/**
 * 
 */
UCLASS()
class NEWGAME_API UCharoverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetHealthbarPercent(float Percent);
	void SetStaminabarPercent(float Percent);
	void SetGold(int32 Gold);
	void SetSoul(int32 Soul);

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgressbar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Goldcount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Soulcount;


};
