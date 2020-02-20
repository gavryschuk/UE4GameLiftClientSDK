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
	Aws::CloudWatchLogs::Model::InputLogEvent LogEvent;
	LogEvent.SetTimestamp(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	LogEvent.SetMessage(TCHAR_TO_UTF8(*Message));
	mInputEvents.push_back(LogEvent);

	// if the previous log is in progress => quit
	if (bIsRunning) return;

	if (LogsClient)
	{
		// log is running
		bIsRunning = true;

		//if Sequence Token is absent => Request Sequence Token
		if (mSequenceToken.Len() == 0) {
			GetSequenceToken();
			return;
		}

		// send logs
		PutLogs();
		return;
	}
	// something went wrong!!!!
	LOG_ERROR("LogsClient is null. Did you call CreateLogsObject and CreateLogsCustomEventObject first?");
#endif
}

void UAWSLogsCustomEventObject::GetSequenceToken()
{
#if WITH_AWS_LOGS
	// generate describeLogStream Request
	Aws::CloudWatchLogs::Model::DescribeLogStreamsRequest StreamsRequest;
	StreamsRequest.SetLogGroupName(TCHAR_TO_UTF8(*GroupName));
	StreamsRequest.SetLogStreamNamePrefix(TCHAR_TO_UTF8(*StreamName));

	// setup handler
	Aws::CloudWatchLogs::DescribeLogStreamsResponseReceivedHandler StreamRequestHandler;
	StreamRequestHandler = std::bind(&UAWSLogsCustomEventObject::OnDescribeLogStreams, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	
	// call DescribeLogStream
	LogsClient->DescribeLogStreamsAsync(StreamsRequest, StreamRequestHandler);
#endif
}

void UAWSLogsCustomEventObject::PutLogs() 
{
#if WITH_AWS_LOGS
	// If InputEvents STack is empty => stop the process
	if (mInputEvents.size() == 0) {
		bIsRunning = false;
		return;
	}
	
	// setup GroupName and StreamName
	Aws::CloudWatchLogs::Model::PutLogEventsRequest LogEventRequest;
	LogEventRequest.SetLogGroupName(TCHAR_TO_UTF8(*GroupName));
	LogEventRequest.SetLogStreamName(TCHAR_TO_UTF8(*StreamName));
	LogEventRequest.SetLogEvents(Aws::Vector<Aws::CloudWatchLogs::Model::InputLogEvent>(mInputEvents)); // log stack is copied into a new aws::vector

	// clear current log stack
	mInputEvents.clear();

	//Add Sequence Token to the request
	if( mSequenceToken.Len() > 0 ) LogEventRequest.SetSequenceToken(TCHAR_TO_UTF8(*mSequenceToken));

	// setup handler
	Aws::CloudWatchLogs::PutLogEventsResponseReceivedHandler PutLogEventHandler;
	PutLogEventHandler = std::bind(&UAWSLogsCustomEventObject::PutLogEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

	// send Custom Log
	LogsClient->PutLogEventsAsync(LogEventRequest, PutLogEventHandler);
#endif
}

void UAWSLogsCustomEventObject::RegisterGroup() 
{
#if WITH_AWS_LOGS
	// if Log Goup is absent (new Log Group)
	if (!bIsGroupCreated) {
		// generate CreateLogGroup Request
		Aws::CloudWatchLogs::Model::CreateLogGroupRequest LogGroupRequest;
		LogGroupRequest.SetLogGroupName(TCHAR_TO_UTF8(*GroupName));
		// setup handler
		Aws::CloudWatchLogs::CreateLogGroupResponseReceivedHandler LogGroupRequestHandler;
		LogGroupRequestHandler = std::bind(&UAWSLogsCustomEventObject::OnCreateLogGroup, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		// call Generate Log Group
		LogsClient->CreateLogGroupAsync(LogGroupRequest, LogGroupRequestHandler);
		return;
	}

	bIsRunning = false;
#endif
}

void UAWSLogsCustomEventObject::RegisterStream()
{
#if WITH_AWS_LOGS
	// if Log Stream is absent (new Log Stream)
	if (!bIsStreamCreated) {
		// generate CreateStreamGroup Request
		Aws::CloudWatchLogs::Model::CreateLogStreamRequest LogStreamRequest;
		LogStreamRequest.SetLogGroupName(TCHAR_TO_UTF8(*GroupName));
		LogStreamRequest.SetLogStreamName(TCHAR_TO_UTF8(*StreamName));
		// setup handler
		Aws::CloudWatchLogs::CreateLogStreamResponseReceivedHandler LogStreamRequestHandler;
		LogStreamRequestHandler = std::bind(&UAWSLogsCustomEventObject::OnCreateLogStream, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		// call Generate Stream Group
		LogsClient->CreateLogStreamAsync(LogStreamRequest, LogStreamRequestHandler);
		return;
	}
	bIsRunning = false;
#endif
}

void UAWSLogsCustomEventObject::OnDescribeLogStreams(const Aws::CloudWatchLogs::CloudWatchLogsClient* Client, const Aws::CloudWatchLogs::Model::DescribeLogStreamsRequest& Request, const Aws::CloudWatchLogs::Model::DescribeLogStreamsOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_AWS_LOGS
	if (!Outcome.IsSuccess()) {
		if (!bIsGroupCreated) RegisterGroup(); // register a group
		else if (!bIsStreamCreated) RegisterStream(); // register a stream
	} else {
		// goup and stream are present on CLoudWatchLogs
		bIsGroupCreated = true;
		bIsStreamCreated = true;
		// setup sequence token
		mSequenceToken = FString(Outcome.GetResult().GetLogStreams()[0].GetUploadSequenceToken().c_str());
		// send logs
		PutLogs();
	}
#endif
}

void UAWSLogsCustomEventObject::OnCreateLogGroup(const Aws::CloudWatchLogs::CloudWatchLogsClient* Client, const Aws::CloudWatchLogs::Model::CreateLogGroupRequest& Request, const Aws::CloudWatchLogs::Model::CreateLogGroupOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_AWS_LOGS
	if (!Outcome.IsSuccess()) {
		LOG_ERROR(FString::Printf(TEXT("Log Group Was Not Registered: %s"), *FString(Outcome.GetError().GetMessage().c_str())));
		// stop the log
		bIsRunning = false;
	} else {
		bIsGroupCreated = true;
		// register stream
		RegisterStream();
	}
#endif
}

void UAWSLogsCustomEventObject::OnCreateLogStream(const Aws::CloudWatchLogs::CloudWatchLogsClient* Client, const Aws::CloudWatchLogs::Model::CreateLogStreamRequest& Request, const Aws::CloudWatchLogs::Model::CreateLogStreamOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_AWS_LOGS
	if (!Outcome.IsSuccess()) {
		LOG_ERROR(FString::Printf(TEXT("Log Stream Was Not Registered: %s"), *FString(Outcome.GetError().GetMessage().c_str())));
		// stop the log
		bIsRunning = false;
	} else {
		bIsStreamCreated = true;
		// send logs
		PutLogs();
	}
#endif
}

void UAWSLogsCustomEventObject::PutLogEvent(const Aws::CloudWatchLogs::CloudWatchLogsClient* Client, const Aws::CloudWatchLogs::Model::PutLogEventsRequest& Request, const Aws::CloudWatchLogs::Model::PutLogEventsOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_AWS_LOGS
	if (!Outcome.IsSuccess()) {
		// we are failed!
		LOG_ERROR(FString::Printf(TEXT("PutLogEvent Error: %s"), *FString(Outcome.GetError().GetMessage().c_str())));
	} else {
		// get sequence token for next request
		mSequenceToken = FString(Outcome.GetResult().GetNextSequenceToken().c_str());
	}
	// stop the log
	bIsRunning = false;
#endif
}