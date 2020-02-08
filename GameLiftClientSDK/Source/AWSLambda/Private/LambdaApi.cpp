// Fill out your copyright notice in the Description page of Project Settings.

#include "LambdaApi.h"
#include "LambdaGlobals.h"

#if WITH_AWS_LAMBDA
#include <aws/core/utils/Outcome.h>
#include <aws/lambda/LambdaClient.h>
#include <aws/lambda/model/InvokeRequest.h>
#include <aws/lambda/model/InvokeResult.h>
#endif

ULambdaFunction* ULambdaFunction::CreateLambdaFunction(FString LambdaFunctionName)
{
#if WITH_AWS_LAMBDA
	ULambdaFunction* Proxy = NewObject<ULambdaFunction>();
	Proxy->LambdaFunctionName = LambdaFunctionName;
	return Proxy;
#endif
	return nullptr;
}

bool ULambdaFunction::Call()
{
#if WITH_AWS_LAMBDA
	if (LambdaClient && LambdaFunctionName.Len() > 0)
	{
		LOG_LAMBDA_NORMAL("Preparing to request Lambda...");

		if (OnLambdaFunctionSuccess.IsBound() == false)
		{
			LOG_LAMBDA_ERROR("No functions were bound to OnLambdaFunctionSuccess multi-cast delegate! Aborting Activate.");
			return false;
		}

		if (OnLambdaFunctionFailed.IsBound() == false)
		{
			LOG_LAMBDA_ERROR("No functions were bound to OnLambdaFunctionFailed multi-cast delegate! Aborting Activate.");
			return false;
		}

		Aws::Lambda::Model::InvokeRequest InvokeRequest;
		InvokeRequest.SetFunctionName(TCHAR_TO_UTF8(*LambdaFunctionName));
		InvokeRequest.SetInvocationType(Aws::Lambda::Model::InvocationType::RequestResponse);

		Aws::Lambda::InvokeResponseReceivedHandler Handler;
		Handler = std::bind(&ULambdaFunction::OnFunctionCall, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_LAMBDA_NORMAL("Lambda Request is in progress...");
		LambdaClient->InvokeAsync(InvokeRequest, Handler);
		return true;
	}
	LOG_LAMBDA_ERROR("LambdaClient is null. Or no Lambda Function Name specified. Did you call CreateLambdaObject and CreateLambdaFunction first?");
#endif
	return false;
}

bool ULambdaFunction::CallWithInputParams(const TArray<FLambdaParamsItem>& RequestParams)
{
#if WITH_AWS_LAMBDA
	if (LambdaClient && LambdaFunctionName.Len() > 0)
	{
		LOG_LAMBDA_NORMAL("Preparing to request Lambda...");

		if (OnLambdaFunctionSuccess.IsBound() == false)
		{
			LOG_LAMBDA_ERROR("No functions were bound to OnLambdaFunctionSuccess multi-cast delegate! Aborting Activate.");
			return false;
		}

		if (OnLambdaFunctionFailed.IsBound() == false)
		{
			LOG_LAMBDA_ERROR("No functions were bound to OnLambdaFunctionFailed multi-cast delegate! Aborting Activate.");
			return false;
		}

		Aws::Lambda::Model::InvokeRequest InvokeRequest;
		InvokeRequest.SetFunctionName(TCHAR_TO_UTF8(*LambdaFunctionName));
		InvokeRequest.SetInvocationType(Aws::Lambda::Model::InvocationType::RequestResponse);

		if (RequestParams.Num() > 0)
		{
			LOG_LAMBDA_NORMAL("Request Params detected. Adding Params to request...");

			std::shared_ptr<Aws::IOStream> Payload = Aws::MakeShared<Aws::StringStream>("LambdaFunctionRequest");
			Aws::Utils::Json::JsonValue JsonPayload;

			for (auto& item : RequestParams) {
				JsonPayload.WithString(TCHAR_TO_UTF8(*item.Key), TCHAR_TO_UTF8(*item.Value));
			}

			*Payload << JsonPayload.View().WriteReadable();
			InvokeRequest.SetBody(Payload);
			InvokeRequest.SetContentType("application/javascript");
	}

		Aws::Lambda::InvokeResponseReceivedHandler Handler;
		Handler = std::bind(&ULambdaFunction::OnFunctionCall, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_LAMBDA_NORMAL("Lambda Request is in progress...");
		LambdaClient->InvokeAsync(InvokeRequest, Handler);
		return true;
}
	LOG_LAMBDA_ERROR("LambdaClient is null. Or no Lambda Function Name specified. Did you call CreateLambdaObject and CreateLambdaFunction first?");
#endif
	return false;
}

void ULambdaFunction::OnFunctionCall(const Aws::Lambda::LambdaClient* Client, const Aws::Lambda::Model::InvokeRequest& Request, const Aws::Lambda::Model::InvokeOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_AWS_LAMBDA
	if (Outcome.IsSuccess())
	{
		LOG_LAMBDA_NORMAL("Received OnFunctionCall with Success outcome.");

		LOG_LAMBDA_NORMAL("Parsing result....");
		auto& Result = Outcome.GetResult();
		
		Aws::Utils::Json::JsonValue ResultPayload{ ((Aws::Lambda::Model::InvokeResult&)Result).GetPayload() };
		auto JsonView = ResultPayload.View();

		TArray<FLambdaParamsItem> ResponseArray;

		for (const auto& item : JsonView.GetAllObjects()) {
			auto Key = FString(item.first.c_str());
			auto Value = FString( item.second.AsString().c_str());
			if (item.second.IsIntegerType()) Value = FString::FromInt(item.second.AsInteger());
			ResponseArray.Add(FLambdaParamsItem(Key,Value));
			LOG_LAMBDA_NORMAL(FString::Printf(TEXT("%s - %s"), *Key, *Value));
		}

		const TArray<FLambdaParamsItem> ResponseArrayCopy = TArray<FLambdaParamsItem>(ResponseArray);
		OnLambdaFunctionSuccess.Broadcast(ResponseArrayCopy);
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_LAMBDA_ERROR("Received Lambda OnFunctionCall with failed outcome. Error: " + MyErrorMessage);
		OnLambdaFunctionFailed.Broadcast(MyErrorMessage);
	}
#endif
}