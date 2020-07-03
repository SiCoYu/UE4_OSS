// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilFunctionLibrary.h"
#include "Windows/WindowsPlatformMisc.h"

FString UUtilFunctionLibrary::GetAPPGUID()
{
	FString GUID = "";
#if PLATFORM_WINDOWS
	GUID = UUtilFunctionLibrary::GetMacAddress();
#elif PLATFORM_ANDROID
	GUID = "/storage/emulated/legacy/AnpanMMO/";
#elif PLATFORM_IOS
	GUID = NSHomeDirectory() + "/Library/Application Support/AnpanMMO/;
#endif
	return GUID;
}

FString UUtilFunctionLibrary::GetMacAddress()
{
	const int MAC_ADDRESS_BYTE_LEN = 6;
	TArray<uint8> MacAddr;
	MacAddr.AddZeroed(MAC_ADDRESS_BYTE_LEN);

	TArray<uint8> Addresses = FWindowsPlatformMisc::GetMacAddress();

	FString Result;
	for (TArray<uint8>::TConstIterator it(Addresses); it; ++it)
	{
		Result += FString::Printf(TEXT("%02x"), *it);
	}
	return Result;
}


uint64 UUtilFunctionLibrary::GetAPP64GUID()
{
	return 0;
}