// Fill out your copyright notice in the Description page of Project Settings.


#include "OSSFunctionLibrary.h"
#include "SimpleOSSManage.h"
#include "PathFunctionLibrary.h"
#include "Engine/Engine.h"
#include "UtilFunctionLibrary.h"
#include "SimpleOSSConfig.h"

bool UOSSFunctionLibrary::UploadOSSFiles()
{
	FString AccessKeyId = GetDefault<USimpleOSSConfig>()->AccessKeyId;
	FString AccessKeySecret = GetDefault<USimpleOSSConfig>()->AccessKeySecret;
	FString EndPoint = GetDefault<USimpleOSSConfig>()->EndPoint;
	FString BucketName = GetDefault<USimpleOSSConfig>()->BucketName;
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, EndPoint);
	UploadFiles(EOSSType::SaveGames);
	UploadFiles(EOSSType::JsonFiles);
	UploadFiles(EOSSType::Crashes);
	return true;
}

bool UOSSFunctionLibrary::DownloadOSSFiles()
{
	FString AccessKeyId = GetDefault<USimpleOSSConfig>()->AccessKeyId;
	FString AccessKeySecret = GetDefault<USimpleOSSConfig>()->AccessKeySecret;
	FString EndPoint = GetDefault<USimpleOSSConfig>()->EndPoint;
	FString BucketName = GetDefault<USimpleOSSConfig>()->BucketName;
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, EndPoint);	
	DownloadFiles(EOSSType::SaveGames);
	DownloadFiles(EOSSType::JsonFiles);
	DownloadFiles(EOSSType::Crashes);
	return true;
}

bool UOSSFunctionLibrary::UploadFiles(const EOSSType OType)
{
	FOSSDirectoryInfo DirectoryInfo;
	for (auto itr : GetDefault<USimpleOSSConfig>()->OSSDirectoryConfigs)
	{
		if (itr.OSSType == OType)
		{
			DirectoryInfo = itr;
			break;
		}
	}
	TArray<FString> ObjectName;
	UPathFunctionLibrary::OSS_GetAllFiles(OType, ObjectName);
	FString SavedPath = UPathFunctionLibrary::SCPPaths__SavedDir();
	for (auto tmp : ObjectName)
	{
		FString FileNameWithExt = FPaths::GetCleanFilename(tmp);
		FString MacAddr = UUtilFunctionLibrary::GetAPPGUID();
		MacAddr.ToUpperInline();
		FString LocalPath = FPaths::Combine(SavedPath, DirectoryInfo.Name, FileNameWithExt);
		FString RemotePath = FPaths::Combine(MacAddr, DirectoryInfo.Name, FileNameWithExt);
		if (OType == EOSSType::Crashes)
		{
			int32 index = LocalPath.Len();
			FileNameWithExt = tmp.Right(index);
			RemotePath = FPaths::Combine(MacAddr, DirectoryInfo.Name, FileNameWithExt);
		}
		EOSSLoadType LoadType = GetDefault<USimpleOSSConfig>()->OSSLoadType;
		if (LoadType == EOSSLoadType::UploadOnly || LoadType == EOSSLoadType::UpAndDownload)
		{
			FString BucketName = GetDefault<USimpleOSSConfig>()->BucketName;
			UE_LOG(LogTemp, Log, TEXT("Upload RemotePath = %s"), *RemotePath);
			SIMPLE_OSS.PutObject(BucketName, tmp, RemotePath);
		}
		
	}
	return true;
}

bool UOSSFunctionLibrary::DownloadFiles(const EOSSType OType)
{
	FOSSDirectoryInfo DirectoryInfo;
	for (auto itr : GetDefault<USimpleOSSConfig>()->OSSDirectoryConfigs)
	{
		if (itr.OSSType == OType)
		{
			DirectoryInfo = itr;
			break;
		}
	}
	FString MacAddr = UUtilFunctionLibrary::GetAPPGUID();
	MacAddr.ToUpperInline();
	FString RemotePath = FPaths::Combine(MacAddr, DirectoryInfo.Name, TEXT("/"));
	TArray<FOSSObjectInfo> ObjectName;
	FString BucketName = GetDefault<USimpleOSSConfig>()->BucketName;
	SIMPLE_OSS.ListObjectsByPrefix(BucketName, RemotePath, ObjectName);
	FString SaveFilePath = UPathFunctionLibrary::SCPPaths__SavedDir();
	for (auto &tmp : ObjectName)
	{
		if (SIMPLE_OSS.DoesObjectExist(BucketName, tmp.Name))
		{
			FClientConfiguration ClientConfiguration;
			SIMPLE_OSS.InitConf(ClientConfiguration);
			FString LocalPath = FPaths::Combine(SaveFilePath, tmp.Name);
			FString LocalBasePath = FPaths::GetPath(LocalPath);
			bool is_exist = UPathFunctionLibrary::CheckDirectory(LocalBasePath);
			if (is_exist)
			{
				EOSSLoadType LoadType = GetDefault<USimpleOSSConfig>()->OSSLoadType;
				if (LoadType == EOSSLoadType::DownloadOnly || LoadType == EOSSLoadType::UpAndDownload)
				{
					UE_LOG(LogTemp, Log, TEXT("Download LocalPath = %s"), *LocalPath);
					SIMPLE_OSS.AsyncGetObjectToLocal(BucketName, tmp.Name, LocalPath);
				}				
			}
		}
	}
	return true;
}

bool UOSSFunctionLibrary::FileIO__SaveStringTextToFile(FString SaveDirectory, FString JoyfulFileName, FString SaveText, bool AllowOverWriting /*= false*/)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*SaveDirectory))
	{
		return false;
	}
	SaveDirectory += "\\";
	SaveDirectory += JoyfulFileName;
	if (!AllowOverWriting)
	{
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
		{
			return false;
		}
	}
	return FFileHelper::SaveStringToFile(SaveText, *SaveDirectory);
}

bool UOSSFunctionLibrary::FileIO__SaveStringArrayToFile(FString SaveDirectory, FString JoyfulFileName, TArray<FString> SaveText, bool AllowOverWriting /*= false*/)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*SaveDirectory))
	{
		return false;
	}
	SaveDirectory += "\\";
	SaveDirectory += JoyfulFileName;
	if (!AllowOverWriting)
	{
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
		{
			return false;
		}
	}
	FString FinalStr = "";
	for (FString& Each : SaveText)
	{
		FinalStr += Each;
		FinalStr += LINE_TERMINATOR;
	}
	return FFileHelper::SaveStringToFile(FinalStr, *SaveDirectory);
}
