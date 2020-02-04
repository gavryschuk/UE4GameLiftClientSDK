// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LambdaBFL.generated.h"

/**
 * 
 */
UCLASS()
class GAMELIFTCLIENTSDK_API ULambdaBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "GameLift MakeLambdaCall")
	static void MakeLambdaCall();
};
