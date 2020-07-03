// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjectXGameMode.h"
#include "Engine.h"
#include "ProjectXHUD.h"
#include "ProjectXCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "OSSFunctionLibrary.h"
#include "UtilFunctionLibrary.h"
#include "Misc/Guid.h"

AProjectXGameMode::AProjectXGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AProjectXHUD::StaticClass();
}

void AProjectXGameMode::BeginPlay()
{
	Super::BeginPlay();
	TArray<int32> IndexArray;
	/*IndexArray[100] = 0;
	UE_LOG(LogTemp, Error, TEXT("BackIntrace test"));*/
	UOSSFunctionLibrary::UploadOSSFiles();
	UOSSFunctionLibrary::DownloadOSSFiles();
	
}