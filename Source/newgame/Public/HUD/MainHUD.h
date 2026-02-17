// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

/**
 * 
 */

class UCharoverlay;

UCLASS()
class NEWGAME_API AMainHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = Char)
	TSubclassOf<UCharoverlay> Charoverlay;

	UPROPERTY()
	UCharoverlay* Greyoverlay;

public: 
	FORCEINLINE UCharoverlay* GetGreyoverlay() const { return Greyoverlay; }

};
