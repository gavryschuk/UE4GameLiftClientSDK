// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#if WITH_AWS_LOGS
#include <aws/logs/CloudWatchLogsClient.h>
#endif
#include "AWSLogsApi.generated.h"

UCLASS()
class AWSLOGS_API UAWSLogsCustomEventObject : public UObject
{
	GENERATED_BODY()
	friend class UAWSLogsObject;
private:
	Aws::CloudWatchLogs::CloudWatchLogsClient* LogsClient;
	FString SequenceToken;

	static UAWSLogsCustomEventObject* CreateLogsCustomEvent();

public:
	UFUNCTION(BlueprintCallable, Category = "Logs Custom Event")
	void Call(const FString& GroupName, const FString& StreamName, const FString& Message);
};