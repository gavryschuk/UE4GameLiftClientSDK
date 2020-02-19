// Fill out your copyright notice in the Description page of Project Settings.

#include "AWSCloudWatchObject.h"
#if WITH_AWS_CLOUDWATCH
#include <aws/core/auth/AWSCredentialsProvider.h>
#endif

void UAWSCloudWatchObject::Internal_InitCloudWatch(const FString& AccessKey, const FString& Secret, const FString& Region)
{
#if WITH_AWS_CLOUDWATCH
	Aws::Client::ClientConfiguration ClientConfig;
	Aws::Auth::AWSCredentials Credentials;

	ClientConfig.connectTimeoutMs = 10000;
	ClientConfig.requestTimeoutMs = 10000;
	ClientConfig.region = TCHAR_TO_UTF8(*Region);

	Credentials = Aws::Auth::AWSCredentials(TCHAR_TO_UTF8(*AccessKey), TCHAR_TO_UTF8(*Secret));	
	CloudWatchClient = new Aws::CloudWatch::CloudWatchClient(Credentials, ClientConfig);
#endif
}

UAWSCloudWatchObject* UAWSCloudWatchObject::CreateCloudWatchObject(const FString& AccessKey, const FString& Secret, const FString& Region /*= "us-east-1"*/)
{
#if WITH_AWS_CLOUDWATCH
	UAWSCloudWatchObject* Proxy = NewObject<UAWSCloudWatchObject>();
	Proxy->Internal_InitCloudWatch(AccessKey, Secret, Region);
	return Proxy;
#endif
	return nullptr;
}

UAWSCloudWatchCustomMetricsObject* UAWSCloudWatchObject::CreateCloudWatchCustomMetricsObject()
 {
 #if WITH_AWS_CLOUDWATCH
	UAWSCloudWatchCustomMetricsObject* Proxy = UAWSCloudWatchCustomMetricsObject::CreateCloudWatchCustomMetrics();
 	Proxy->CloudWatchClient = CloudWatchClient;
 	return Proxy;
 #endif
 	return nullptr;
 }