// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#if WITH_AWS_CLOUDWATCH
#include <aws/monitoring/CloudWatchClient.h>
#endif
#include "AWSCloudWatchApi.generated.h"

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
class AWSCLOUDWATCH_API UAWSCloudWatchCustomMetricsObject : public UObject
{
	GENERATED_BODY()
	friend class UAWSCloudWatchObject;
public:
	UPROPERTY(BlueprintAssignable, Category = "Cloud Watch Custom Metrics")
	FOnAWSCloudWatchCustomMetricsSuccess OnAWSCloudWatchCustomMetricsSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Cloud Watch Custom Metrics")
	FOnAWSCloudWatchCustomMetricsFailed OnAWSCloudWatchCustomMetricsFailed;
private:
	Aws::CloudWatch::CloudWatchClient* CloudWatchClient;

	static UAWSCloudWatchCustomMetricsObject* CreateCloudWatchCustomMetrics();

public:
	UFUNCTION(BlueprintCallable, Category = "Cloud Watch Custom Metrics")
	void Call(const FAWSCloudWatchCustomMetricsConfig& CloudWatchCustomMetricsData );
private:
	void OnCustomMetricsCall(const Aws::CloudWatch::CloudWatchClient* Client, const Aws::CloudWatch::Model::PutMetricDataRequest& Request, const Aws::CloudWatch::Model::PutMetricDataOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};