// Fill out your copyright notice in the Description page of Project Settings.

#include "AWSCloudWatchApi.h"
#include "GameLiftClientSDK/Public/GameLiftClientGlobals.h"

#if WITH_AWS_CLOUDWATCH
#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/monitoring/model/PutMetricDataRequest.h>
#endif

UAWSCloudWatchCustomMetricsObject* UAWSCloudWatchCustomMetricsObject::CreateCloudWatchCustomMetrics(const FString& NameSpace, const FString& GroupName)
{
#if WITH_AWS_CLOUDWATCH
	UAWSCloudWatchCustomMetricsObject* Proxy = NewObject<UAWSCloudWatchCustomMetricsObject>();
	Proxy->NameSpace = NameSpace;
	Proxy->GroupName = GroupName;
	return Proxy;
#endif
	return nullptr;
}

void UAWSCloudWatchCustomMetricsObject::Call(const FString& KeyName, const FString& ValueName, const float Value)
{
#if WITH_AWS_CLOUDWATCH
	// if the previous custom metrics log is in progress => quit
	if (bIsRunning) {
		LOG_NORMAL("Previous Custom Metrics call is in progress. Make next next call after responce is recieved");
		return;
	}

	if (CloudWatchClient)
	{
		bIsRunning = true;

		Aws::CloudWatch::Model::Dimension dimension;
		dimension.SetName(TCHAR_TO_UTF8(*GroupName));
		dimension.SetValue(TCHAR_TO_UTF8(*KeyName));

		Aws::CloudWatch::Model::MetricDatum datum;
		datum.SetMetricName(TCHAR_TO_UTF8(*ValueName));
		datum.SetUnit(Aws::CloudWatch::Model::StandardUnit::None);
		datum.SetValue(static_cast<double>(Value));
		datum.AddDimensions(dimension);

		Aws::CloudWatch::Model::PutMetricDataRequest MetricDataRequest;
		MetricDataRequest.SetNamespace(TCHAR_TO_UTF8(*NameSpace));
		MetricDataRequest.AddMetricData(datum);
		
		// send data without callbacks
		if (OnAWSCloudWatchCustomMetricsSuccess.IsBound() == false || OnAWSCloudWatchCustomMetricsFailed.IsBound() == false) {
			CloudWatchClient->PutMetricData(MetricDataRequest);
			bIsRunning = false;
			return;
		}

		// callbacks functions are present. Send Custom metrics with Callbacks handler
		Aws::CloudWatch::PutMetricDataResponseReceivedHandler Handler;
		Handler = std::bind(&UAWSCloudWatchCustomMetricsObject::OnCustomMetricsCall, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		CloudWatchClient->PutMetricDataAsync(MetricDataRequest, Handler);
	}
	LOG_ERROR("CloudWatchClient is null. Did you call CreateCloudWatchObject and CreateCloudWatchCustomMetricsObject first?");
#endif
}

void UAWSCloudWatchCustomMetricsObject::OnCustomMetricsCall(const Aws::CloudWatch::CloudWatchClient* Client, const Aws::CloudWatch::Model::PutMetricDataRequest& Request, const Aws::CloudWatch::Model::PutMetricDataOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_AWS_CLOUDWATCH
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnCustomMetricsCall with Success outcome.");
		OnAWSCloudWatchCustomMetricsSuccess.Broadcast();
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_WARNING("Received Cloud Watch OnCustomMetricsCall with failed outcome. Error: " + MyErrorMessage);
		OnAWSCloudWatchCustomMetricsFailed.Broadcast(MyErrorMessage);
	}
	bIsRunning = false;
#endif
}