// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectXGameMode.generated.h"

UCLASS(minimalapi)
class AProjectXGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AProjectXGameMode();

	virtual void BeginPlay();
};



