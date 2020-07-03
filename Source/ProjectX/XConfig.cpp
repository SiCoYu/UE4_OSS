// Fill out your copyright notice in the Description page of Project Settings.


#include "XConfig.h"
#include "Misc/Paths.h"

#if WITH_EDITOR
#define USE_LOCAL_SERVER
#endif

#ifndef USE_LOCAL_SERVER
const FString XConfig::ServerHost = "218.220.121.112";
#else
const FString XConfig::ServerHost = "127.0.0.1";
#endif
const int XConfig::ServerPort = 4424;

#ifndef USE_LOCAL_SERVER
const FString XConfig::MasterURL = "http://153.121.36.165/~Master/anpanmmomaster/main/";
#else
const FString XConfig::MasterURL = "http://153.121.36.165/~Master/anpanmmomaster/local/";
#endif
const FString XConfig::ScriptURL = "http://153.121.36.165/~Master/anpanmmoscript/";
const FString XConfig::DLCURL = "http://153.121.36.165/~Master/anpanmmo/dlc/";

const FString XConfig::AccessKeyId = "123";
const FString XConfig::AccessKeySecret = "456";
const FString XConfig::EndPoint = "beijing.aliyuncs.com";
const FString XConfig::BucketName = "project";