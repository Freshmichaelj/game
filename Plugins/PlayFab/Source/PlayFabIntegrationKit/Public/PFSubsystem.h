//////////////////////////////////////////////////////
// Copyright (C) Betide Studio. 2023. All rights reserved.
//////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "PlayFabJsonObject.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PFSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FPFUploadFileToAzureStorageDelegate, bool, bWasUploadSuccess, int32, ResponseCode);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FPFUploadFileToAzureStorageProgressDelegate, int32, BytesSent, int32, BytesReceived, float, UploadPercentage);
UCLASS()
class PLAYFABINTEGRATIONKIT_API UPFSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPFSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "PlayFab Integration Kit")
	void ConvertFileToBinaryArray(FString FilePath, TArray<uint8>& OutArray);

	UFUNCTION(BlueprintCallable, Category = "PlayFab Integration Kit")
	void UploadFileToAzureStorage(FString URL, TArray<uint8> FileData, FPFUploadFileToAzureStorageProgressDelegate UploadProccess,FPFUploadFileToAzureStorageDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "PlayFab Integration Kit")
	void SelectFileFromLocal(FString& OutFilePath, FString& OutFileName, TArray<uint8>& OutFileContent);
	
	//Force shutdown dedicated server for PlayFab so you don't have to wait for the server to shutdown itself which takes about 15 minutes
	UFUNCTION(BlueprintCallable, Category = "PlayFab Integration Kit")
	void ForceShutdownDedicatedServer();

	UFUNCTION(BlueprintCallable, Category = "PlayFab Integration Kit", meta = (WorldContext = "WorldContextObject"))
	void ConvertStringToURL(UObject* WorldContextObject,FString CustomData, FString& OutURL);

	//Prevent the server from crashing while playing in editor
	UFUNCTION(BlueprintPure, Category = "PlayFab Integration Kit")
	static bool CheckIfPlayFabServerIsActive();

	//Get the Steam Token from the Subsystem
	UFUNCTION(BlueprintPure, Category = "PlayFab Integration Kit")
	static FString GetSteamToken();
	
};
