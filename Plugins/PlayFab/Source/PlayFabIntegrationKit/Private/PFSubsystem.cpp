//////////////////////////////////////////////////////
// Copyright (C) Betide Studio. 2023. All rights reserved.
//////////////////////////////////////////////////////


#include "PFSubsystem.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "HttpModule.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#if WITH_EDITOR
#include "Dialogs/Dialogs.h"
#endif
#include "OnlineSubsystem.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Modules/ModuleManager.h"

UPFSubsystem::UPFSubsystem()
{
}

void UPFSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UPFSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UPFSubsystem::ConvertFileToBinaryArray(FString FilePath, TArray<uint8>& OutArray)
{
	FFileHelper::LoadFileToArray(OutArray, *FilePath);
}

void UPFSubsystem::UploadFileToAzureStorage(FString URL, TArray<uint8> FileData, FPFUploadFileToAzureStorageProgressDelegate UploadProccess,FPFUploadFileToAzureStorageDelegate OnComplete)
{
	TSharedRef<IHttpRequest> JsonReq = FHttpModule::Get().CreateRequest();
	JsonReq->SetVerb("PUT");
	JsonReq->SetURL(URL);
	JsonReq->SetHeader("Content-Type", "application/octet-stream");
	JsonReq->SetHeader("comp", "blob");
	JsonReq->SetHeader("x-ms-blob-type", "BlockBlob");
	JsonReq->SetContent(FileData);
	JsonReq->OnProcessRequestComplete().BindLambda([this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccess)
	{
		if(bWasSuccess && (Response->GetResponseCode() == 201 || Response->GetResponseCode() == 200))
		{
			OnComplete.ExecuteIfBound(true, Response->GetResponseCode());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Upload file to Azure storage failed!"));
			OnComplete.ExecuteIfBound(false, Response->GetResponseCode());
		}
	});
	JsonReq->OnRequestProgress().BindLambda([this,UploadProccess, FileData](FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
	{
		UE_LOG(LogTemp, Warning, TEXT("BytesSent: %d, BytesReceived: %d"), BytesSent, BytesReceived);
		UE_LOG(LogTemp, Warning, TEXT("Upload percentage: %f"), (float)BytesSent / (float)Request->GetContentLength());
		UploadProccess.ExecuteIfBound(BytesSent, BytesReceived, (float)BytesSent / (float)Request->GetContentLength());
	});
	JsonReq->ProcessRequest();
}

void UPFSubsystem::SelectFileFromLocal(FString& OutFilePath, FString& OutFileName, TArray<uint8>& OutFileContent)
{
#if WITH_EDITOR
	if (PromptUserForDirectory(OutFilePath, "Select a file for upload", ""))
	{
		TArray<uint8> OutArray;
		if(FFileHelper::LoadFileToArray(OutArray, *OutFilePath))
		{
			OutFileName = FPaths::GetCleanFilename(OutFilePath);
			OutFileContent = OutArray;
			return;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Load file to array failed!"));
			return;
		}
	}
#endif
}

void UPFSubsystem::ForceShutdownDedicatedServer()
{
	FGenericPlatformMisc::RequestExit(false);
}

void UPFSubsystem::ConvertStringToURL(UObject* WorldContextObject,FString CustomData, FString& OutURL)
{
	if(UPlayFabJsonObject* JsonObj = UPlayFabJsonObject::ConstructJsonObject(WorldContextObject))
	{
		if(JsonObj->DecodeJson(CustomData))
		{
			if(JsonObj->HasField("Icon"))
			{
				OutURL = JsonObj->GetStringField("Icon");
				return;
			}
		}
	}
}

bool UPFSubsystem::CheckIfPlayFabServerIsActive()
{
	if(FModuleManager::Get().ModuleExists(TEXT("MPlayFabGSDK")) || FModuleManager::Get().ModuleExists(TEXT("PlayFabGSDK")))
	{
		return true;
	}
	return false;
}

FString UPFSubsystem::GetSteamToken()
{
	if (const IOnlineSubsystem *OnlineSub = IOnlineSubsystem::Get())
	{
		if (const IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface(); Identity.IsValid())
		{
			if (TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0); UserId.IsValid())
			{
				return Identity->GetAuthToken(0);
			}
			if(Identity->GetAuthToken(0) != "DummyAuthTicket")
			{
				return Identity->GetAuthToken(0);
			}
		}
	}
	return "";
}
