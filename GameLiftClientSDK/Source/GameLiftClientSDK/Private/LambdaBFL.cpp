// Fill out your copyright notice in the Description page of Project Settings.

#include "LambdaBFL.h"

#include <aws/identity-management/auth/PersistentCognitoIdentityProvider.h>
#include <aws/identity-management/auth/CognitoCachingCredentialsProvider.h>

#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>

#include <aws/lambda/LambdaClient.h>
#include <aws/lambda/model/InvokeRequest.h>
#include <aws/lambda/model/InvokeResult.h>


void ULambdaBFL::MakeLambdaCall()
{
	auto credentialsProvider = Aws::MakeShared<Aws::Auth::CognitoCachingAnonymousCredentialsProvider>("CredentialsProvider", "749912423565", "us-east-1:5a926442-6147-46de-8bee-e5fe4b5b2508" );

	static std::shared_ptr<Aws::Lambda::LambdaClient> s_LambdaClient = Aws::MakeShared<Aws::Lambda::LambdaClient>("LambdaClient", credentialsProvider);

	Aws::Lambda::Model::InvokeRequest invokeRequest;
	invokeRequest.SetFunctionName("ConnectClientToServer");
	invokeRequest.SetInvocationType(Aws::Lambda::Model::InvocationType::RequestResponse);

	//std::shared_ptr<Aws::IOStream> payload = Aws::MakeShared<Aws::StringStream>("LambdaFunctionRequest");
	//Aws::Utils::Json::JsonValue jsonPayload;
	//jsonPayload.WithString("param1", param1);
	//jsonPayload.WithInteger("param2", param2);
	//jsonPayload.WithInteger("param3", param3);
	////////*payload << jsonPayload.View().WriteReadable();
	////////invokeRequest.SetBody(payload);
	////////invokeRequest.SetContentType("application/javascript");

	auto outcome = s_LambdaClient->Invoke(invokeRequest);

	if (outcome.IsSuccess())
	{
		auto& result = outcome.GetResult();
		Aws::Utils::Json::JsonValue resultPayload{ result.GetPayload() };
		auto jsonView = resultPayload.View();

		if (jsonView.ValueExists("PlayerSessionId"))
		{
			auto PlayerSessionId = FString(jsonView.GetString("PlayerSessionId").c_str());
			UE_LOG(LogTemp, Warning, TEXT("PlayerSessionId: %s"), *PlayerSessionId);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to parse PlayerSessionId from response"));
		}

		if (jsonView.ValueExists("PlayerId"))
		{
			auto PlayerId = FString(jsonView.GetString("PlayerId").c_str());
			UE_LOG(LogTemp, Warning, TEXT("PlayerId: %s"), *PlayerId);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to parse PlayerId from response"));
		}

		if (jsonView.ValueExists("FleetId"))
		{
			auto FleetId = FString(jsonView.GetString("FleetId").c_str());
			UE_LOG(LogTemp, Warning, TEXT("FleetId: %s"), *FleetId);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to parse FleetId from response"));
		}

		if (jsonView.ValueExists("FleetArn"))
		{
			auto FleetArn = FString(jsonView.GetString("FleetArn").c_str());
			UE_LOG(LogTemp, Warning, TEXT("FleetArn: %s"), *FleetArn);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to parse FleetArn from response"));
		}

		if (jsonView.ValueExists("DnsName"))
		{
			auto DnsName = FString(jsonView.GetString("DnsName").c_str());
			UE_LOG(LogTemp, Warning, TEXT("DnsName response: %s"), *DnsName);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to parse DnsName from response"));
		}

		if (jsonView.ValueExists("Port"))
		{
			auto Port = FString::FromInt(jsonView.GetInteger("Port"));
			UE_LOG(LogTemp, Warning, TEXT("Port response: %s"), *Port);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to parse Port from response"));
		}
	}
	else
	{
		auto error = outcome.GetError();
		auto errorStr = error.GetMessage().c_str();
		UE_LOG(LogTemp, Warning, TEXT("Lambda Error: %s"), *errorStr);
	}
}