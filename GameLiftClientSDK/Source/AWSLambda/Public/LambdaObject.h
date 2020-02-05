// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#if WITH_AWS_LAMBDA
#include <aws/lambda/LambdaClient.h>
#endif
#include "LambdaObject.generated.h"

UCLASS()
class LAMBDA_API ULambdaObject : public UObject
{
	GENERATED_BODY()

private:
#if WITH_AWS_LAMBDA
	Aws::Lambda::LambdaClient* LambdaClient;
#endif
	void Internal_InitLambdaWithUsersPool(const FString& AccessKey, const FString& Secret, const FString& Region);
	void Internal_InitLambdaWithIdentityPool(const FString& AccountID, const FString& CognitoIdentityPoolId, const FString& Region);

public:

	/**
	* public static ULambdaObject::CreateLambdaObjectWithUsersPool
	* Creates a LambdaObject with AWS Account Users Pool. This function must be called first before accessing any Lambda client functions.
	* @param AccessKey [const FString&] AccessKey of your AWS user. @See http://docs.aws.amazon.com/general/latest/gr/managing-aws-access-keys.html
	* @param Secret [const FString&] SecretKey of your AWS user. @See http://docs.aws.amazon.com/general/latest/gr/managing-aws-access-keys.html
	* @param Region [const FString&] Default is set to us-east-1 (North Virginia).
	* @return [ULambdaObject*] Returns UGameLiftClientObject*. Use this to create game sessions, player sessions etc.
	**/
	UFUNCTION(BlueprintCallable, Category = "Lambda Client Object")
	static ULambdaObject* CreateLambdaObjectWithUsersPool(const FString& AccessKey, const FString& Secret, const FString& Region = "us-east-1");

	/**
	* public static ULambdaObject::CreateLambdaObjectWithIdentityPool
	* Creates a LambdaObject with Cognito Identity Pool. This function must be called first before accessing any Lambda client functions.
	* @param AccountID [const FString&] AccountID of your AWS account. @See https://docs.aws.amazon.com/general/latest/gr/acct-identifiers.html
	* @param CognitoIdentityPoolId [const FString&] Cognito Identity Pool Id. @See https://docs.aws.amazon.com/cognito/latest/developerguide/identity-pools.html
	* @return [ULambdaObject*] Returns UGameLiftClientObject*. Use this to create game sessions, player sessions etc.
	**/
	UFUNCTION(BlueprintCallable, Category = "Lambda Client Object")
	static ULambdaObject* CreateLambdaObjectWithIdentityPool(const FString& AccountID, const FString& CognitoIdentityPoolId, const FString& Region = "us-east-1");

	/**
	* public ULambdaFunction::CreateLambdaFunction
	* Creates a LambdaFunction Object. To Make Calls to AWS Lambda
	* @param LambdaFunctionName [FString] Lambda Function Name from your AWS account.
	* @return [ULambdaFunction*] Returns ULambdaFunction*. Use this to Call the function itself and manage response.
	**/
	UFUNCTION(BlueprintCallable, Category = "Lambda Client Object")
	ULambdaFunction* CreateLambdaFunction(FString LambdaFunctionName);
	
};
