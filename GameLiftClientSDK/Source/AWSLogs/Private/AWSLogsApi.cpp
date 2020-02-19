// Fill out your copyright notice in the Description page of Project Settings.

#include "AWSLogsApi.h"
#include "GameLiftClientSDK/Public/GameLiftClientGlobals.h"

#if WITH_AWS_LOGS
#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/logs/model/InputLogEvent.h>
#include <aws/logs/model/PutLogEventsResult.h>
#include <aws/logs/model/PutLogEventsRequest.h>
#endif

UAWSLogsCustomEventObject* UAWSLogsCustomEventObject::CreateLogsCustomEvent()
{
#if WITH_AWS_LOGS
	return NewObject<UAWSLogsCustomEventObject>();
#endif
	return nullptr;
}

void UAWSLogsCustomEventObject::Call(const FString& GroupName, const FString& StreamName, const FString& Message)
{
#if WITH_AWS_LOGS
	if (LogsClient)
	{
		LOG_NORMAL("Sending Cloud Watch Custom Event...");

		Aws::CloudWatchLogs::Model::InputLogEvent LogEvent;
		LogEvent.SetTimestamp(std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1));
		LogEvent.SetMessage(TCHAR_TO_UTF8(*Message));


		Aws::CloudWatchLogs::Model::PutLogEventsRequest LogEventRequest;
		LogEventRequest.SetLogGroupName(TCHAR_TO_UTF8(*GroupName));
		LogEventRequest.SetLogStreamName(TCHAR_TO_UTF8(*StreamName));
		LogEventRequest.AddLogEvents(LogEvent);
		if(SequenceToken.Len() > 0)LogEventRequest.SetSequenceToken(TCHAR_TO_UTF8(*SequenceToken));

		auto Outcome = LogsClient->PutLogEvents(LogEventRequest);

		if (Outcome.IsSuccess()) SequenceToken = FString(Outcome.GetResult().GetNextSequenceToken().c_str());

	}
	LOG_ERROR("LogsClient is null. Did you call CreateLogsObject and CreateLogsCustomEventObject first?");
#endif
}