// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Healthbar.generated.h"

/**
 * 
 */
UCLASS()
class NEWGAME_API UHealthbar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Healthbar;

};

