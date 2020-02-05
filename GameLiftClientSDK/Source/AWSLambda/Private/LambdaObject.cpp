// Fill out your copyright notice in the Description page of Project Settings.

#include "LambdaObject.h"

#include <aws/identity-management/auth/PersistentCognitoIdentityProvider.h>
#include <aws/identity-management/auth/CognitoCachingCredentialsProvider.h>

void ULambdaObject::Internal_InitLambda(const FString& AccountID, const FString& CognitoIdentityPoolId)
{
#if WITH_AWS_LAMBDA
	auto credentialsProvider = Aws::MakeShared<Aws::Auth::CognitoCachingAnonymousCredentialsProvider>("CredentialsProvider", TCHAR_TO_UTF8(*AccountID), TCHAR_TO_UTF8(*CognitoIdentityPoolId));
	LambdaClient = new Aws::Lambda::LambdaClient(credentialsProvider);
#endif
}

ULambdaObject* ULambdaObject::CreateLambdaObject(const FString& AccountID, const FString& CognitoIdentityPoolId)
{
#if WITH_AWS_LAMBDA
	ULambdaObject* Proxy = NewObject<ULambdaObject>();
	Proxy->Internal_InitLambda(AccountID, CognitoIdentityPoolId);
	return Proxy;
#endif
	return nullptr;
}

ULambdaFunction* ULambdaObject::CreateLambdaFunction(FString LambdaFunctionName)
{
#if WITH_AWS_LAMBDA
	ULambdaFunction* Proxy = ULambdaFunction::CreateLambdaFunction(LambdaFunctionName);
	Proxy->LambdaClient = LambdaClient;
	return Proxy;
#endif
	return nullptr;
}