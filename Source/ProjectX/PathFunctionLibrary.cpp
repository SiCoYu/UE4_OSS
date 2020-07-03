// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFunctionLibrary.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/FileManager.h"

template <class FunctorType>
class PlatformFileFunctor : public IPlatformFile::FDirectoryVisitor	//GenericPlatformFile.h
{
public:

	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
	{
		return Functor(FilenameOrDirectory, bIsDirectory);
	}

	PlatformFileFunctor(FunctorType&& FunctorInstance)
		: Functor(MoveTemp(FunctorInstance))
	{
	}

private:
	FunctorType Functor;
};

template <class Functor>
PlatformFileFunctor<Functor> MakeDirectoryVisitor(Functor&& FunctorInstance)
{
	return PlatformFileFunctor<Functor>(MoveTemp(FunctorInstance));
}

static FDateTime GetFileTimeStamp(const FString& File)
{
	return FPlatformFileManager::Get().GetPlatformFile().GetTimeStamp(*File);
}
static void SetTimeStamp(const FString& File, const FDateTime& TimeStamp)
{
	FPlatformFileManager::Get().GetPlatformFile().SetTimeStamp(*File, TimeStamp);
}

//Radial Result Struct
struct FFileStampSort
{
	FString* FileName;
	FDateTime FileStamp;

	FFileStampSort(FString* IN_Name, FDateTime Stamp)
		: FileName(IN_Name)
		, FileStamp(Stamp)
	{}
};

FORCEINLINE bool operator< (const FFileStampSort& Left, const FFileStampSort& Right)
{
	return Left.FileStamp < Right.FileStamp;
}

static bool GetFiles(const FString& FullPathOfBaseDir, TArray<FString>& FilenamesOut, bool Recursive = false, const FString& FilterByExtension = "")
{
	const FString FileExt = FilterByExtension.Replace(TEXT("."), TEXT("")).ToLower();
	FString Str;
	auto FilenamesVisitor = MakeDirectoryVisitor(
		[&](const TCHAR* FilenameOrDirectory, bool bIsDirectory)
	{
		if (!bIsDirectory)
		{
			if (FileExt != "")
			{
				Str = FPaths::GetCleanFilename(FilenameOrDirectory);	
				if (FPaths::GetExtension(Str).ToLower() == FileExt)
				{
					if (Recursive)
					{
						FilenamesOut.Push(FilenameOrDirectory); //need whole path for recursive
					}
					else
					{
						FilenamesOut.Push(Str);
					}
				}
			}
			else
			{
				Str = FPaths::GetCleanFilename(FilenameOrDirectory);
				if (Recursive)
				{
					FilenamesOut.Push(FilenameOrDirectory); //need whole path for recursive
				}
				else
				{
					FilenamesOut.Push(Str);
				}
			}
		}
		return true;
	}
	);
	if (Recursive)
	{
		return FPlatformFileManager::Get().GetPlatformFile().IterateDirectoryRecursively(*FullPathOfBaseDir, FilenamesVisitor);
	}
	else
	{
		return FPlatformFileManager::Get().GetPlatformFile().IterateDirectory(*FullPathOfBaseDir, FilenamesVisitor);
	}
}

static bool FileExists(const FString& File)
{
	return FPlatformFileManager::Get().GetPlatformFile().FileExists(*File);
}
static bool FolderExists(const FString& Dir)
{
	return FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*Dir);
}
static bool RenameFile(const FString& Dest, const FString& Source)
{
	//Make sure file modification time gets updated!
	SetTimeStamp(Source, FDateTime::Now());

	return FPlatformFileManager::Get().GetPlatformFile().MoveFile(*Dest, *Source);
}

static bool VCreateDirectory(FString FolderToMake)
{
	if (FolderExists(FolderToMake))
	{
		return true;
	}
	FPaths::NormalizeDirectoryName(FolderToMake);
	FolderToMake += "/";
	FString Base;
	FString Left;
	FString Remaining;
	FolderToMake.Split(TEXT("/"), &Base, &Remaining);
	Base += "/";
	while (Remaining != "")
	{
		Remaining.Split(TEXT("/"), &Left, &Remaining);
		Base += Left + FString("/");

		if (!FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*Base) &&
			!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*Base))
		{
			return false;
		}
	}
	return true;
}

void UPathFunctionLibrary::OSS_GetAllFiles(EOSSType OType, TArray<FString>& FileNames, bool Recursive /*= true*/)
{
	FileNames.Empty();
	FString Path = "";
	switch (OType)
	{
	case EOSSType::SaveGames:
		Path = SCPPaths__SavedDir() + "SaveGames";
		break;
	case EOSSType::JsonFiles:
		Path = SCPPaths__SaveJsonDir();
		break;
	case EOSSType::Crashes:
		Path = SCPPaths__CrashesDir();
		break;
	case EOSSType::Logs:
		break;
	default:
		break;
	}
	bool is_success = false;
	if (!Path.IsEmpty())
	{
		is_success = GetFiles(Path, FileNames, Recursive);
	}
	if (!is_success)
	{
		UE_LOG(LogTemp, Error, TEXT("Get OSS Files Failed, OType = %d"), (int32)(OType));
	}
}

FString UPathFunctionLibrary::SCPPaths__Win64Dir_BinaryLocation()
{
	FString Path = "";
#if PLATFORM_WINDOWS
	Path = FString(FPlatformProcess::BaseDir());
#elif PLATFORM_ANDROID
	Path = "/storage/emulated/legacy/AnpanMMO/";
#elif PLATFORM_IOS
	Path = NSHomeDirectory() + "/Library/Application Support/AnpanMMO/;
#endif
	return FPaths::ConvertRelativePathToFull(Path);
}

FString UPathFunctionLibrary::SCPPaths__WindowsNoEditorDir()
{
	FString Path = "";
#if PLATFORM_WINDOWS
	Path = FPaths::RootDir();
#elif PLATFORM_ANDROID
	Path = "/storage/emulated/legacy/AnpanMMO/";
#elif PLATFORM_IOS
	Path = NSHomeDirectory() + "/Library/Application Support/AnpanMMO/;
#endif
	return FPaths::ConvertRelativePathToFull(Path);
}

FString UPathFunctionLibrary::SCPPaths__GameRootDirectory()
{
	FString Path = "";
#if PLATFORM_WINDOWS
	Path = FPaths::ProjectDir();
#elif PLATFORM_ANDROID
	Path = "/storage/emulated/legacy/AnpanMMO/";
#elif PLATFORM_IOS
	Path = NSHomeDirectory() + "/Library/Application Support/AnpanMMO/;
#endif
	return FPaths::ConvertRelativePathToFull(Path);
}

FString UPathFunctionLibrary::SCPPaths__SavedDir()
{
	FString Path = "";
#if PLATFORM_WINDOWS
	Path = FPaths::ProjectSavedDir();
#elif PLATFORM_ANDROID
	Path = "/storage/emulated/legacy/AnpanMMO/";
#elif PLATFORM_IOS
	Path = NSHomeDirectory() + "/Library/Application Support/AnpanMMO/;
#endif
	return FPaths::ConvertRelativePathToFull(Path);
}

FString UPathFunctionLibrary::SCPPaths__ConfigDir()
{
	FString Path = "";
#if PLATFORM_WINDOWS
	Path = FPaths::ProjectConfigDir();
#elif PLATFORM_ANDROID
	Path = "/storage/emulated/legacy/AnpanMMO/";
#elif PLATFORM_IOS
	Path = NSHomeDirectory() + "/Library/Application Support/AnpanMMO/;
#endif
	return FPaths::ConvertRelativePathToFull(Path);
}

FString UPathFunctionLibrary::SCPPaths__ScreenShotsDir()
{
	FString Path = "";
#if PLATFORM_WINDOWS
	Path = FPaths::ScreenShotDir();
#elif PLATFORM_ANDROID
	Path = "/storage/emulated/legacy/AnpanMMO/";
#elif PLATFORM_IOS
	Path = NSHomeDirectory() + "/Library/Application Support/AnpanMMO/;
#endif
	return FPaths::ConvertRelativePathToFull(Path);
}

FString UPathFunctionLibrary::SCPPaths__LogsDir()
{
	FString Path = "";
#if PLATFORM_WINDOWS
	Path = FPaths::ProjectLogDir();
#elif PLATFORM_ANDROID
	Path = "/storage/emulated/legacy/AnpanMMO/";
#elif PLATFORM_IOS
	Path = NSHomeDirectory() + "/Library/Application Support/AnpanMMO/;
#endif
	return FPaths::ConvertRelativePathToFull(Path);
}

FString UPathFunctionLibrary::SCPPaths__SaveJsonDir()
{
	FString Path = "";
#if PLATFORM_WINDOWS
	Path = FPaths::ProjectSavedDir();
#elif PLATFORM_ANDROID
	Path = "/storage/emulated/legacy/AnpanMMO/";
#elif PLATFORM_IOS
	Path = NSHomeDirectory() + "/Library/Application Support/AnpanMMO/;
#endif
	Path = FPaths::Combine(Path, TEXT("JsonFiles"));
	return FPaths::ConvertRelativePathToFull(Path);
}

FString UPathFunctionLibrary::SCPPaths__CrashesDir()
{
	FString Path = "";
#if PLATFORM_WINDOWS
	Path = FPaths::ProjectSavedDir();
#elif PLATFORM_ANDROID
	Path = "/storage/emulated/legacy/AnpanMMO/";
#elif PLATFORM_IOS
	Path = NSHomeDirectory() + "/Library/Application Support/AnpanMMO/;
#endif
	Path = FPaths::Combine(Path, TEXT("Crashes"));
	return FPaths::ConvertRelativePathToFull(Path);
}

bool UPathFunctionLibrary::CheckDirectory(const FString& LocalPath)
{
	if (FPaths::DirectoryExists(LocalPath))
	{
		return true;
	}
	if (!LocalPath.IsEmpty())
	{
		return FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*LocalPath);
	}
	return false;
}
