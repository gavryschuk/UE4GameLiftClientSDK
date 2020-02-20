// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#if WITH_AWS_CLOUDWATCH
#include <aws/monitoring/CloudWatchClient.h>
#endif
#include "AWSCloudWatchApi.generated.h"

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
	FString NameSpace;
	FString GroupName;

	bool bIsRunning = false;

	static UAWSCloudWatchCustomMetricsObject* CreateCloudWatchCustomMetrics(const FString& NameSpace, const FString& GroupName);

public:
	UFUNCTION(BlueprintCallable, Category = "Cloud Watch Custom Metrics")
	void Call(const FString& KeyName, const FString& ValueName, const float Value );
private:
	void OnCustomMetricsCall(const Aws::CloudWatch::CloudWatchClient* Client, const Aws::CloudWatch::Model::PutMetricDataRequest& Request, const Aws::CloudWatch::Model::PutMetricDataOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};