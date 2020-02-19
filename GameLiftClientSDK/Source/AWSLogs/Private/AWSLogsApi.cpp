// Fill out your copyright notice in the Description page of Project Settings.

#include "AWSLogsApi.h"
#include "GameLiftClientSDK/Public/GameLiftClientGlobals.h"

#if WITH_AWS_LOGS
#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#endif

UAWSLogsCustomEventObject* UAWSLogsCustomEventObject::CreateLogsCustomEvent(const FString& GroupName, const FString& StreamName)
{
#if WITH_AWS_LOGS
	UAWSLogsCustomEventObject* Proxy = NewObject<UAWSLogsCustomEventObject>();
	Proxy->GroupName = GroupName;
	Proxy->StreamName = StreamName;
	return Proxy;
#endif
	return nullptr;
}

void UAWSLogsCustomEventObject::Call(const FString& Message)
{
#if WITH_AWS_LOGS
	// add messages to the log
	mMessageArray.Add(FAWSLogsMessageItem(Message));
	// if the previous log is in progress => quit
	if (bIsRunning) return;

	if (LogsClient)
	{
		LOG_NORMAL("Sending Cloud Watch Custom Event...");
		
		bIsRunning = true;

		//if Sequence Token is absent => Request Sequence Token
		if (mSequenceToken.Len() == 0) {

			Aws::CloudWatchLogs::Model::DescribeLogStreamsRequest StreamsRequest;
			StreamsRequest.SetLogGroupName(TCHAR_TO_UTF8(*GroupName));
			StreamsRequest.SetLogStreamNamePrefix(TCHAR_TO_UTF8(*StreamName));

			Aws::CloudWatchLogs::DescribeLogStreamsResponseReceivedHandler StreamRequestHandler;
			StreamRequestHandler = std::bind(&UAWSLogsCustomEventObject::OnDescribeLogStreams, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
			LogsClient->DescribeLogStreamsAsync(StreamsRequest, StreamRequestHandler);
			return;
		}

		// setup GroupName and StreamName
		Aws::CloudWatchLogs::Model::PutLogEventsRequest LogEventRequest;
		LogEventRequest.SetLogGroupName(TCHAR_TO_UTF8(*GroupName));
		LogEventRequest.SetLogStreamName(TCHAR_TO_UTF8(*StreamName));

		// setup log timestamp and message
		for (int32 Index = (mMessageArray.Num() - 1); Index >=0; Index--)
		{
			Aws::CloudWatchLogs::Model::InputLogEvent LogEvent;
			
			uint64 unixTime = 0;
			mMessageArray[Index].TimeStamp.FromUnixTimestamp(unixTime);
			unixTime *= 1000;

			LogEvent.SetTimestamp(static_cast<long long>(unixTime));
			LogEvent.SetMessage(TCHAR_TO_UTF8(*mMessageArray[Index].Message));
			LogEventRequest.AddLogEvents(LogEvent);
			mMessageArray.RemoveAt(Index);
		}

		//Add Sequence Token to the request
		LogEventRequest.SetSequenceToken(TCHAR_TO_UTF8(*mSequenceToken));

		// send Custom Log
		Aws::CloudWatchLogs::PutLogEventsResponseReceivedHandler PutLogEventHandler;
		PutLogEventHandler = std::bind(&UAWSLogsCustomEventObject::PutLogEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LogsClient->PutLogEventsAsync(LogEventRequest, PutLogEventHandler);
	}
	LOG_ERROR("LogsClient is null. Did you call CreateLogsObject and CreateLogsCustomEventObject first?");
#endif
}

void UAWSLogsCustomEventObject::RegisterGroup() 
{
	if (!bIsGroupCreated) {
		Aws::CloudWatchLogs::Model::CreateLogGroupRequest LogGroupRequest;
		LogGroupRequest.SetLogGroupName(TCHAR_TO_UTF8(*GroupName));

		Aws::CloudWatchLogs::CreateLogGroupResponseReceivedHandler LogGroupRequestHandler;
		LogGroupRequestHandler = std::bind(&UAWSLogsCustomEventObject::OnCreateLogGroup, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);


		LogsClient->CreateLogGroupAsync(LogGroupRequest, LogGroupRequestHandler);
		return;
	}

	bIsRunning = false;
}

void UAWSLogsCustomEventObject::RegisterStream()
{
	if (!bIsStreamCreated) {

		Aws::CloudWatchLogs::Model::CreateLogStreamRequest LogStreamRequest;
		LogStreamRequest.SetLogGroupName(TCHAR_TO_UTF8(*GroupName));
		LogStreamRequest.SetLogStreamName(TCHAR_TO_UTF8(*StreamName));

		Aws::CloudWatchLogs::CreateLogStreamResponseReceivedHandler LogStreamRequestHandler;
		LogStreamRequestHandler = std::bind(&UAWSLogsCustomEventObject::OnCreateLogStream, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LogsClient->CreateLogStreamAsync(LogStreamRequest, LogStreamRequestHandler);
		return;
	}
	bIsRunning = false;
}

void UAWSLogsCustomEventObject::OnDescribeLogStreams(const Aws::CloudWatchLogs::CloudWatchLogsClient* Client, const Aws::CloudWatchLogs::Model::DescribeLogStreamsRequest& Request, const Aws::CloudWatchLogs::Model::DescribeLogStreamsOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
	if (!Outcome.IsSuccess()) {
		if (!bIsGroupCreated) {
			RegisterGroup();
			return;
		}
		else if (!bIsStreamCreated) {
			RegisterStream();
			return;
		}
	}else {
		bIsGroupCreated = true;
		bIsStreamCreated = true;

		mSequenceToken = FString(Outcome.GetResult().GetNextToken().c_str());

		bIsRunning = false;
		return;
	}
}

void UAWSLogsCustomEventObject::OnCreateLogGroup(const Aws::CloudWatchLogs::CloudWatchLogsClient* Client, const Aws::CloudWatchLogs::Model::CreateLogGroupRequest& Request, const Aws::CloudWatchLogs::Model::CreateLogGroupOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
	if (!Outcome.IsSuccess()) {
		LOG_ERROR(FString::Printf(TEXT("Log Group Was Not Registered: %s"), *FString(Outcome.GetError().GetMessage().c_str())));
		bIsRunning = false;
		return;
	}
	else {
		bIsGroupCreated = true;
		RegisterStream();
		return;
	}
}

void UAWSLogsCustomEventObject::OnCreateLogStream(const Aws::CloudWatchLogs::CloudWatchLogsClient* Client, const Aws::CloudWatchLogs::Model::CreateLogStreamRequest& Request, const Aws::CloudWatchLogs::Model::CreateLogStreamOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
	if (!Outcome.IsSuccess()) {
		LOG_ERROR(FString::Printf(TEXT("Log Stream Was Not Registered: %s"), *FString(Outcome.GetError().GetMessage().c_str())));
		bIsRunning = false;
		return;
	}
	else {
		bIsStreamCreated = true;
		bIsRunning = false;
		return;
	}

}

void UAWSLogsCustomEventObject::PutLogEvent(const Aws::CloudWatchLogs::CloudWatchLogsClient* Client, const Aws::CloudWatchLogs::Model::PutLogEventsRequest& Request, const Aws::CloudWatchLogs::Model::PutLogEventsOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
	
	if (!Outcome.IsSuccess()) {
		LOG_ERROR(FString::Printf(TEXT("PutLogEvent Error: %s"), *FString(Outcome.GetError().GetMessage().c_str())));
		bIsRunning = false;
		return;
	}
	else {
		mSequenceToken = FString(Outcome.GetResult().GetNextSequenceToken().c_str());
		bIsRunning = false;
		return;
	}
}