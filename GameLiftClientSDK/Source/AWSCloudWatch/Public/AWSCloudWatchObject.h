// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AWSCloudWatchApi.h"
#if WITH_AWS_CLOUDWATCH
#include <aws/monitoring/CloudWatchClient.h>
#endif
#include "AWSCloudWatchObject.generated.h"

UCLASS()
class AWSCLOUDWATCH_API UAWSCloudWatchObject : public UObject
{
	GENERATED_BODY()

private:

	void Internal_InitCloudWatch(const FString& AccessKey, const FString& Secret, const FString& Region);
	Aws::CloudWatch::CloudWatchClient* CloudWatchClient;
public:

	/**
	* public static UAWSCloudWatchObject::CreateCloudWatchObject
	* Creates a CloudWatchObject . This function must be called first before accessing any Cloud Watch client functions.
	* @param AccessKey [const FString&] AccessKey of your AWS user. @See http://docs.aws.amazon.com/general/latest/gr/managing-aws-access-keys.html
	* @param Secret [const FString&] SecretKey of your AWS user. @See http://docs.aws.amazon.com/general/latest/gr/managing-aws-access-keys.html
	* @param Region [const FString&] Default is set to us-east-1 (North Virginia).
	* @return [UAWSCloudWatchObject*] Returns UAWSCloudWatchObject*. Use this to create custom logs to Cloud Watch service.
	**/
	UFUNCTION(BlueprintCallable, Category = "Cloud Watch Client Object")
	static UAWSCloudWatchObject* CreateCloudWatchObject(const FString& AccessKey, const FString& Secret, const FString& Region = "us-east-1");

    
	/**
	* public UAWSCloudWatchCustomMetricsObject::CreateCloudWatchCustomMetricsObject
	* Creates aCloud Watch Custom Metrics Object. To Send Custom Metrics
	* @return [UAWSCloudWatchCustomMetricsObject*] Returns UAWSCloudWatchCustomMetricsObject*. Use this to Send Custom Metrics and manage response.
	**/
	UFUNCTION(BlueprintCallable, Category = "Cloud Watch Custom Metrics Object")
	UAWSCloudWatchCustomMetricsObject* CreateCloudWatchCustomMetricsObject(const FString& NameSpace, const FString& GroupName);
};