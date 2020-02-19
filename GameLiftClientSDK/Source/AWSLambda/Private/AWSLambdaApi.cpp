// Fill out your copyright notice in the Description page of Project Settings.

#include "AWSLambdaApi.h"
#include "GameLiftClientSDK/Public/GameLiftClientGlobals.h"

#if WITH_AWS_LAMBDA
#include <aws/core/utils/Outcome.h>
#include <aws/lambda/LambdaClient.h>
#include <aws/lambda/model/InvokeRequest.h>
#include <aws/lambda/model/InvokeResult.h>
#endif

UAWSLambdaFunction* UAWSLambdaFunction::CreateLambdaFunction(FString LambdaFunctionName)
{
#if WITH_AWS_LAMBDA
	UAWSLambdaFunction* Proxy = NewObject<UAWSLambdaFunction>();
	Proxy->LambdaFunctionName = LambdaFunctionName;
	return Proxy;
#endif
	return nullptr;
}

void UAWSLambdaFunction::Call(const TArray<FAWSLambdaParamsItem>& RequestParams)
{
#if WITH_AWS_LAMBDA
	if (LambdaClient && LambdaFunctionName.Len() > 0)
	{
		LOG_NORMAL("Preparing to request Lambda...");

		Aws::Lambda::Model::InvokeRequest InvokeRequest;
		InvokeRequest.SetFunctionName(TCHAR_TO_UTF8(*LambdaFunctionName));
		InvokeRequest.SetInvocationType(Aws::Lambda::Model::InvocationType::RequestResponse);

		if (RequestParams.Num() > 0)
		{
			LOG_NORMAL("Request Params detected. Adding Params to request...");

			std::shared_ptr<Aws::IOStream> Payload = Aws::MakeShared<Aws::StringStream>("LambdaFunctionRequest");
			Aws::Utils::Json::JsonValue JsonPayload;

			for (auto& item : RequestParams) {
				JsonPayload.WithString(TCHAR_TO_UTF8(*item.Key), TCHAR_TO_UTF8(*item.Value));
			}

			*Payload << JsonPayload.View().WriteReadable();
			InvokeRequest.SetBody(Payload);
			InvokeRequest.SetContentType("application/javascript");
		}

		// send data without callbacks
		if (OnAWSLambdaFunctionSuccess.IsBound() == false || OnAWSLambdaFunctionFailed.IsBound() == false) {
			LOG_NORMAL("At least one callback function is not provided. Lambda calls without callbacks...");
			LambdaClient->Invoke(InvokeRequest);
			return;
		}

		Aws::Lambda::InvokeResponseReceivedHandler Handler;
		Handler = std::bind(&UAWSLambdaFunction::OnFunctionCall, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Lambda Async call. Callbacks provided");
		LambdaClient->InvokeAsync(InvokeRequest, Handler);
	}
	LOG_ERROR("LambdaClient is null. Or no Lambda Function Name specified. Did you call CreateLambdaObject and CreateLambdaFunction first?");
#endif
}

void UAWSLambdaFunction::OnFunctionCall(const Aws::Lambda::LambdaClient* Client, const Aws::Lambda::Model::InvokeRequest& Request, const Aws::Lambda::Model::InvokeOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_AWS_LAMBDA
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnFunctionCall with Success outcome.");

		LOG_NORMAL("Parsing result....");
		auto& Result = Outcome.GetResult();
		
		Aws::Utils::Json::JsonValue ResultPayload{ ((Aws::Lambda::Model::InvokeResult&)Result).GetPayload() };
		auto JsonView = ResultPayload.View();

		TArray<FAWSLambdaParamsItem> ResponseArray;

		bool bIsError = false;

		for (const auto& item : JsonView.GetAllObjects()) {
			auto Key = FString(item.first.c_str());
			auto Value = FString( item.second.AsString().c_str());

			if (Key == "errorMessage") {
				LOG_ERROR("Received Lambda OnFunctionCall with Error Message. Error: " + Value);
				OnAWSLambdaFunctionFailed.Broadcast(Value);
				return;
			}

			if (item.second.IsIntegerType()) Value = FString::FromInt(item.second.AsInteger());
			ResponseArray.Add(FAWSLambdaParamsItem(Key,Value));
			LOG_NORMAL(FString::Printf(TEXT("%s - %s"), *Key, *Value));
		}

		const TArray<FAWSLambdaParamsItem> ResponseArrayCopy = TArray<FAWSLambdaParamsItem>(ResponseArray);
		OnAWSLambdaFunctionSuccess.Broadcast(ResponseArrayCopy);
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received Lambda OnFunctionCall with failed outcome. Error: " + MyErrorMessage);
		OnAWSLambdaFunctionFailed.Broadcast(MyErrorMessage);
	}
#endif
}