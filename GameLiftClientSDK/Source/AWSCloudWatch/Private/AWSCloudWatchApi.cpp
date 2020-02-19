// Fill out your copyright notice in the Description page of Project Settings.

#include "AWSCloudWatchApi.h"
#include "GameLiftClientSDK/Public/GameLiftClientGlobals.h"

#if WITH_AWS_CLOUDWATCH
#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/monitoring/model/PutMetricDataRequest.h>
#endif

UAWSCloudWatchCustomMetricsObject* UAWSCloudWatchCustomMetricsObject::CreateCloudWatchCustomMetrics()
{
#if WITH_AWS_CLOUDWATCH
	return NewObject<UAWSCloudWatchCustomMetricsObject>();
#endif
	return nullptr;
}

void UAWSCloudWatchCustomMetricsObject::Call(const FAWSCloudWatchCustomMetricsConfig& CloudWatchCustomMetricsData)
{
#if WITH_AWS_CLOUDWATCH
	if (CloudWatchClient)
	{
		LOG_NORMAL("Preparing to send Cloud Watch Custom Metrics...");

		Aws::CloudWatch::Model::Dimension dimension;
		dimension.SetName(TCHAR_TO_UTF8(*CloudWatchCustomMetricsData.MetricsGroupName));
		dimension.SetValue(TCHAR_TO_UTF8(*CloudWatchCustomMetricsData.MetricKeyName));

		Aws::CloudWatch::Model::MetricDatum datum;
		datum.SetMetricName(TCHAR_TO_UTF8(*CloudWatchCustomMetricsData.MetricValueName));
		datum.SetUnit(Aws::CloudWatch::Model::StandardUnit::None);
		datum.SetValue(static_cast<double>(CloudWatchCustomMetricsData.Value));
		datum.AddDimensions(dimension);

		Aws::CloudWatch::Model::PutMetricDataRequest MetricDataRequest;
		MetricDataRequest.SetNamespace(TCHAR_TO_UTF8(*CloudWatchCustomMetricsData.NameSpace));
		MetricDataRequest.AddMetricData(datum);
		
		// send data without callbacks
		if (OnAWSCloudWatchCustomMetricsSuccess.IsBound() == false || OnAWSCloudWatchCustomMetricsFailed.IsBound() == false) {
			LOG_NORMAL("At least one callback function is not provided. Cloud Watch sends Custom Metrics without callbacks...");
			CloudWatchClient->PutMetricData(MetricDataRequest);
			return;
		}

		// callbacks functions are present. Send Custom metrics with Callbacks handler
		Aws::CloudWatch::PutMetricDataResponseReceivedHandler Handler;
		Handler = std::bind(&UAWSCloudWatchCustomMetricsObject::OnCustomMetricsCall, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Cloud Watch sends Custom Metrics Async. Callbacks provided.");
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
		LOG_ERROR("Received Cloud Watch OnCustomMetricsCall with failed outcome. Error: " + MyErrorMessage);
		OnAWSCloudWatchCustomMetricsFailed.Broadcast(MyErrorMessage);
	}
#endif
}