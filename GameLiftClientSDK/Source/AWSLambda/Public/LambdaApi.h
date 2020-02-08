// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#if WITH_AWS_LAMBDA
#include "aws/lambda/LambdaClient.h"
#endif
#include "LambdaApi.generated.h"


USTRUCT(Blueprintable, BlueprintType)
struct FLambdaParamsItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Key;

	UPROPERTY(BlueprintReadWrite)
	FString Value;

	FLambdaParamsItem()
	{
		Key = "";
		Value = "";
	}

	FLambdaParamsItem(FString InitKey, FString InitValue )
	{
		Key = InitKey;
		Value = InitValue;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLambdaFunctionSuccess, const TArray<FLambdaParamsItem>&, ResponseArray);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLambdaFunctionFailed, const FString&, ErrorMessage);
UCLASS()
class LAMBDA_API ULambdaFunction : public UObject
{
	GENERATED_BODY()

	friend class ULambdaObject;
public:
	UPROPERTY(BlueprintAssignable, Category = "Lambda Function")
	FOnLambdaFunctionSuccess OnLambdaFunctionSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Lambda Function")
	FOnLambdaFunctionFailed OnLambdaFunctionFailed;
private:
	Aws::Lambda::LambdaClient* LambdaClient;
	FString LambdaFunctionName;

	static ULambdaFunction* CreateLambdaFunction(FString LambdaFunctionName);
public:
	UFUNCTION(BlueprintCallable, Category = "Lambda Function")
	bool Call();

	UFUNCTION(BlueprintCallable, Category = "Lambda Function")
	bool CallWithInputParams(const TArray<FLambdaParamsItem>& RequestParams);

private:
	void OnFunctionCall(const Aws::Lambda::LambdaClient* Client, const Aws::Lambda::Model::InvokeRequest& Request, const Aws::Lambda::Model::InvokeOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
	
};

