// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AWSLogsApi.h"
#if WITH_AWS_LOGS
#include <aws/logs/CloudWatchLogsClient.h>
#endif
#include "AWSLogsObject.generated.h"

UCLASS()
class AWSLOGS_API UAWSLogsObject : public UObject
{
	GENERATED_BODY()

private:

	void Internal_InitLogs(const FString& AccessKey, const FString& Secret, const FString& Region);
	Aws::CloudWatchLogs::CloudWatchLogsClient* LogsClient;
public:

	/**
	* public static UAWSLogsObject::CreateLogsObject
	* Creates a CloudWatch Logs Object . This function must be called first before accessing any Cloud Watch Logs client functions.
	* @param AccessKey [const FString&] AccessKey of your AWS user. @See http://docs.aws.amazon.com/general/latest/gr/managing-aws-access-keys.html
	* @param Secret [const FString&] SecretKey of your AWS user. @See http://docs.aws.amazon.com/general/latest/gr/managing-aws-access-keys.html
	* @param Region [const FString&] Default is set to us-east-1 (North Virginia).
	* @return [UAWSLogsObject*] Returns UAWSLogsObject*. Use this to create custom logs to Cloud Watch service.
	**/
	UFUNCTION(BlueprintCallable, Category = "Logs Client Object")
	static UAWSLogsObject* CreateLogsObject(const FString& AccessKey, const FString& Secret, const FString& Region = "us-east-1");


	/**
	* public UAWSLogsObject::CreateLogsCustomEventObject
	* Creates aCloud Watch Custom Metrics Object. To Send Custom Metrics
	* @return [UAWSLogsCustomEventObject*] Returns UAWSLogsCustomEventObject*. Use this to Send Custom Logs.
	**/
	UFUNCTION(BlueprintCallable, Category = "Logs Client Custom Event Object")
	UAWSLogsCustomEventObject* CreateLogsCustomEventObject();
};