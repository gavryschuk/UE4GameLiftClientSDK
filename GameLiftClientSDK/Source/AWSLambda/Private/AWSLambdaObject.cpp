// Fill out your copyright notice in the Description page of Project Settings.

#include "AWSLambdaObject.h"

#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/auth/AWSCredentialsProvider.h>

#include <aws/identity-management/auth/PersistentCognitoIdentityProvider.h>
#include <aws/identity-management/auth/CognitoCachingCredentialsProvider.h>

void UAWSLambdaObject::Internal_InitLambdaWithIAMCredentials(const FString& AccessKey, const FString& Secret, const FString& Region)
{
#if WITH_AWS_LAMBDA
	Aws::Client::ClientConfiguration ClientConfig;
	Aws::Auth::AWSCredentials Credentials;

	ClientConfig.connectTimeoutMs = 10000;
	ClientConfig.requestTimeoutMs = 10000;
	ClientConfig.region = TCHAR_TO_UTF8(*Region);

	Credentials = Aws::Auth::AWSCredentials(TCHAR_TO_UTF8(*AccessKey), TCHAR_TO_UTF8(*Secret));
	LambdaClient = new Aws::Lambda::LambdaClient(Credentials, ClientConfig);
#endif
}

void UAWSLambdaObject::Internal_InitLambdaWithIdentityPool(const FString& AccountID, const FString& CognitoIdentityPoolId, const FString& Region)
{
#if WITH_AWS_LAMBDA
	Aws::Client::ClientConfiguration ClientConfig;

	ClientConfig.connectTimeoutMs = 10000;
	ClientConfig.requestTimeoutMs = 10000;
	ClientConfig.region = TCHAR_TO_UTF8(*Region);

	auto CredentialsProvider = Aws::MakeShared<Aws::Auth::CognitoCachingAnonymousCredentialsProvider>("CredentialsProvider", TCHAR_TO_UTF8(*AccountID), TCHAR_TO_UTF8(*CognitoIdentityPoolId));
	LambdaClient = new Aws::Lambda::LambdaClient(CredentialsProvider, ClientConfig);
#endif
}

UAWSLambdaObject* UAWSLambdaObject::CreateLambdaObjectWithIAMCredentials(const FString& AccessKey, const FString& Secret, const FString& Region /*= "us-east-1"*/)
{
#if WITH_AWS_LAMBDA
	UAWSLambdaObject* Proxy = NewObject<UAWSLambdaObject>();
	Proxy->Internal_InitLambdaWithIAMCredentials(AccessKey, Secret, Region);
	return Proxy;
#endif
	return nullptr;
}

UAWSLambdaObject* UAWSLambdaObject::CreateLambdaObjectWithIdentityPool(const FString& AccountID, const FString& CognitoIdentityPoolId, const FString& Region /*= "us-east-1"*/)
{
#if WITH_AWS_LAMBDA
	UAWSLambdaObject* Proxy = NewObject<UAWSLambdaObject>();
	Proxy->Internal_InitLambdaWithIdentityPool(AccountID, CognitoIdentityPoolId, Region);
	return Proxy;
#endif
	return nullptr;
}

UAWSLambdaFunction* UAWSLambdaObject::CreateLambdaFunction(FString LambdaFunctionName)
{
#if WITH_AWS_LAMBDA
	UAWSLambdaFunction* Proxy = UAWSLambdaFunction::CreateLambdaFunction(LambdaFunctionName);
	Proxy->LambdaClient = LambdaClient;
	return Proxy;
#endif
	return nullptr;
}