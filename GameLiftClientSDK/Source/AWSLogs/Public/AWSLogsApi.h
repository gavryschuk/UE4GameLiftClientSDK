// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/DateTime.h"
#if WITH_AWS_LOGS
#include <aws/logs/CloudWatchLogsClient.h>
#include <aws/logs/model/DescribeLogStreamsRequest.h>
#include <aws/logs/model/DescribeLogStreamsResult.h>
#include <aws/logs/model/InputLogEvent.h>
#include <aws/logs/model/CreateLogGroupRequest.h>
#include <aws/logs/model/CreateLogStreamRequest.h>
#include <aws/logs/model/PutLogEventsRequest.h>
#include <aws/logs/model/PutLogEventsResult.h>
#endif
#include "AWSLogsApi.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FAWSLogsMessageItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FDateTime TimeStamp;

	UPROPERTY(BlueprintReadWrite)
	FString Message;

	FAWSLogsMessageItem()
	{
		TimeStamp = FDateTime::UtcNow();
		Message = "EmptyMessage";
	}

	FAWSLogsMessageItem(FString InitMessage)
	{
		TimeStamp = FDateTime::UtcNow();
		Message = InitMessage;
	}
};

UCLASS()
class AWSLOGS_API UAWSLogsCustomEventObject : public UObject
{
	GENERATED_BODY()
	friend class UAWSLogsObject;

private:
	Aws::CloudWatchLogs::CloudWatchLogsClient* LogsClient;
	FString GroupName;
	FString StreamName;
	
	FString mSequenceToken;
	TArray<FAWSLogsMessageItem> mMessageArray;
	
	bool bIsGroupCreated = false;
	bool bIsStreamCreated = false;
	bool bIsRunning = false;

	static UAWSLogsCustomEventObject* CreateLogsCustomEvent(const FString& GroupName, const FString& StreamName);
public:
	UFUNCTION(BlueprintCallable, Category = "Logs Custom Event")
	void Call(const FString& Message);

private:
	void RegisterGroup();
	void RegisterStream();

	void OnDescribeLogStreams(const Aws::CloudWatchLogs::CloudWatchLogsClient* Client, const Aws::CloudWatchLogs::Model::DescribeLogStreamsRequest& Request, const Aws::CloudWatchLogs::Model::DescribeLogStreamsOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
	void OnCreateLogGroup(const Aws::CloudWatchLogs::CloudWatchLogsClient* Client, const Aws::CloudWatchLogs::Model::CreateLogGroupRequest& Request, const Aws::CloudWatchLogs::Model::CreateLogGroupOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
	void OnCreateLogStream(const Aws::CloudWatchLogs::CloudWatchLogsClient* Client, const Aws::CloudWatchLogs::Model::CreateLogStreamRequest& Request, const Aws::CloudWatchLogs::Model::CreateLogStreamOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
	void PutLogEvent(const Aws::CloudWatchLogs::CloudWatchLogsClient* Client, const Aws::CloudWatchLogs::Model::PutLogEventsRequest& Request, const Aws::CloudWatchLogs::Model::PutLogEventsOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};