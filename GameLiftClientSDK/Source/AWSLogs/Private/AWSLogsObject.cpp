// Fill out your copyright notice in the Description page of Project Settings.

#include "AWSLogsObject.h"
#if WITH_AWS_LOGS
#include <aws/core/auth/AWSCredentialsProvider.h>
#endif

void UAWSLogsObject::Internal_InitLogs(const FString& AccessKey, const FString& Secret, const FString& Region)
{
#if WITH_AWS_LOGS
	Aws::Client::ClientConfiguration ClientConfig;
	Aws::Auth::AWSCredentials Credentials;

	ClientConfig.connectTimeoutMs = 10000;
	ClientConfig.requestTimeoutMs = 10000;
	ClientConfig.region = TCHAR_TO_UTF8(*Region);

	Credentials = Aws::Auth::AWSCredentials(TCHAR_TO_UTF8(*AccessKey), TCHAR_TO_UTF8(*Secret));
	LogsClient = new Aws::CloudWatchLogs::CloudWatchLogsClient(Credentials, ClientConfig);
#endif
}

UAWSLogsObject* UAWSLogsObject::CreateLogsObject(const FString& AccessKey, const FString& Secret, const FString& Region /*= "us-east-1"*/)
{
#if WITH_AWS_LOGS
	UAWSLogsObject* Proxy = NewObject<UAWSLogsObject>();
	Proxy->Internal_InitLogs(AccessKey, Secret, Region);
	return Proxy;
#endif
	return nullptr;
}

UAWSLogsCustomEventObject*  UAWSLogsObject::CreateLogsCustomEventObject()
{
#if WITH_AWS_LOGS
	UAWSLogsCustomEventObject* Proxy = UAWSLogsCustomEventObject::CreateLogsCustomEvent();
	Proxy->LogsClient = LogsClient;
	return Proxy;
#endif
	return nullptr;
}