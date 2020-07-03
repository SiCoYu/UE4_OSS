// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleOSSConfig.h"
#include "PathFunctionLibrary.generated.h"

UCLASS()
class PROJECTX_API UPathFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "PathLibrary|OSS")
	static void OSS_GetAllFiles(EOSSType OType, TArray<FString>& FileNames, bool Recursive = true);
	
	/** InstallDir/GameName/Binaries/Win64 */
	UFUNCTION(BlueprintPure, Category = "PathLibrary|Paths")
	static FString SCPPaths__Win64Dir_BinaryLocation();

	/** InstallDir/WindowsNoEditor/ */
	UFUNCTION(BlueprintPure, Category = "PathLibrary|Paths")
	static FString SCPPaths__WindowsNoEditorDir();

	/** InstallDir/GameName */
	UFUNCTION(BlueprintPure, Category = "PathLibrary|Paths")
	static FString SCPPaths__GameRootDirectory();

	/** InstallDir/GameName/Saved */
	UFUNCTION(BlueprintPure, Category = "PathLibrary|Paths")
	static FString SCPPaths__SavedDir();

	/** InstallDir/GameName/Saved/Config/ */
	UFUNCTION(BlueprintPure, Category = "PathLibrary|Paths")
	static FString SCPPaths__ConfigDir();

	/** InstallDir/GameName/Saved/Screenshots/Windows */
	UFUNCTION(BlueprintPure, Category = "PathLibrary|Paths")
	static FString SCPPaths__ScreenShotsDir();

	/** InstallDir/GameName/Saved/Logs */
	UFUNCTION(BlueprintPure, Category = "PathLibrary|Paths")
	static FString SCPPaths__LogsDir();

	/** InstallDir/GameName/Saved/Crashes */
	UFUNCTION(BlueprintPure, Category = "PathLibrary|Paths")
	static FString SCPPaths__CrashesDir();

	/** InstallDir/GameName/Saved/JsonFiles */
	UFUNCTION(BlueprintPure, Category = "PathLibrary|Paths")
	static FString SCPPaths__SaveJsonDir();

	UFUNCTION()
	static bool CheckDirectory(const FString& LocalPath);
};
