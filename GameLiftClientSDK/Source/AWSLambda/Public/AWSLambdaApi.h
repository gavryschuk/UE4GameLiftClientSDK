// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#if WITH_AWS_LAMBDA
#include "aws/lambda/LambdaClient.h"
#endif
#include "AWSLambdaApi.generated.h"


USTRUCT(Blueprintable, BlueprintType)
struct FAWSLambdaParamsItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Key;

	UPROPERTY(BlueprintReadWrite)
	FString Value;

	FAWSLambdaParamsItem()
	{
		Key = "";
		Value = "";
	}

	FAWSLambdaParamsItem(FString InitKey, FString InitValue )
	{
		Key = InitKey;
		Value = InitValue;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAWSLambdaFunctionSuccess, const TArray<FAWSLambdaParamsItem>&, ResponseArray);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAWSLambdaFunctionFailed, const FString&, ErrorMessage);
UCLASS()
class AWSLAMBDA_API UAWSLambdaFunction : public UObject
{
	GENERATED_BODY()

	friend class UAWSLambdaObject;
public:
	UPROPERTY(BlueprintAssignable, Category = "Lambda Function")
	FOnAWSLambdaFunctionSuccess OnAWSLambdaFunctionSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Lambda Function")
	FOnAWSLambdaFunctionFailed OnAWSLambdaFunctionFailed;
private:
	Aws::Lambda::LambdaClient* LambdaClient;
	FString LambdaFunctionName;

	static UAWSLambdaFunction* CreateLambdaFunction(FString LambdaFunctionName);
public:
	UFUNCTION(BlueprintCallable, Category = "Lambda Function")
	void Call(const TArray<FAWSLambdaParamsItem>& RequestParams = []);

private:
	void OnFunctionCall(const Aws::Lambda::LambdaClient* Client, const Aws::Lambda::Model::InvokeRequest& Request, const Aws::Lambda::Model::InvokeOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
	
};

