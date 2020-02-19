// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AWSCloudWatchApi.h"
#if WITH_AWS_CLOUDWATCH
#include <aws/monitoring/CloudWatchClient.h>
#endif
#include "AWSCloudWatchObject.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FAWSCloudWatchCustomMetricsConfig
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		FString NameSpace;

	UPROPERTY(BlueprintReadWrite)
		FString MetricsGroupName;

	UPROPERTY(BlueprintReadWrite)
		FString MetricKeyName;

	UPROPERTY(BlueprintReadWrite)
		FString MetricValueName;

	UPROPERTY(BlueprintReadWrite)
		double Value;

	FAWSCloudWatchCustomMetricsConfig()
	{
		NameSpace = "UE4/GameLiftClientSDK";
		MetricsGroupName = "MetricGroupName";
		MetricKeyName = "MetricKeyName";
		MetricValueName = "MetricValueName";
		Value = 0.0;
	}

	FAWSCloudWatchCustomMetricsConfig(FString InitNameSpace, FString InitMetricsGroupName, FString InitMetricKeyName, FString InitMetricValueName, double InitValue)
	{
		NameSpace = InitNameSpace;
		MetricsGroupName = InitMetricsGroupName;
		MetricKeyName = InitMetricKeyName;
		MetricValueName = InitMetricValueName;
		Value = InitValue;
	}
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAWSCloudWatchCustomMetricsSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAWSCloudWatchCustomMetricsFailed, const FString&, ErrorMessage);
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
	* public ULambdaFunction::CreateLambdaFunction
	* Creates a LambdaFunction Object. To Make Calls to AWS Lambda
	* @param LambdaFunctionName [FString] Lambda Function Name from your AWS account.
	* @return [ULambdaFunction*] Returns ULambdaFunction*. Use this to Call the function itself and manage response.
	**/
	UFUNCTION(BlueprintCallable, Category = "Lambda Client Object")
	UAWSCloudWatchCustomMetricsObject* CreateCloudWatchCustomMetricsObject();
};