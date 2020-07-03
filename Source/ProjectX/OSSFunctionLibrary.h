// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine.h"
#include "PathFunctionLibrary.h"
#include "SimpleOSSConfig.h"
#include "OSSFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API UOSSFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "OSSLibrary|UploadFile")
	static bool UploadOSSFiles();
	UFUNCTION(BlueprintPure, Category = "OSSLibrary|DownloadFile")
	static bool DownloadOSSFiles();

	UFUNCTION(BlueprintPure, Category = "OSSLibrary|UploadFile")
	static bool UploadFiles(const EOSSType OType);
	UFUNCTION(BlueprintPure, Category = "OSSLibrary|DownloadFile")
	static bool DownloadFiles(const EOSSType OType);

	/** Saves text to filename of your choosing, make sure include whichever file extension you want in the filename, ex: SelfNotes.txt . Make sure to include the entire file path in the save directory, ex: C:\MyGameDir\BPSavedTextFiles */
	UFUNCTION(BlueprintCallable, Category = "OSSLibrary|File IO")
	static bool FileIO__SaveStringTextToFile(FString SaveDirectory, FString JoyfulFileName, FString SaveText, bool AllowOverWriting = false);

	/** Saves multiple Strings to filename of your choosing, with each string on its own line! Make sure include whichever file extension you want in the filename, ex: SelfNotes.txt . Make sure to include the entire file path in the save directory, ex: C:\MyGameDir\BPSavedTextFiles */
	UFUNCTION(BlueprintCallable, Category = "OSSLibrary|File IO")
	static bool FileIO__SaveStringArrayToFile(FString SaveDirectory, FString JoyfulFileName, TArray<FString> SaveText, bool AllowOverWriting = false);
};
