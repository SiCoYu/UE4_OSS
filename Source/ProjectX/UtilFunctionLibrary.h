// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API UUtilFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static FString GetAPPGUID();
	static FString GetMacAddress();

	static uint64 GetAPP64GUID();

};
