#include "AWSLambdaModulePrivatePCH.h"
#include "AWSLambdaModule.h"
#include "GameLiftClientSDK/Public/GameLiftClientGlobals.h"
#include "IPluginManager.h"

#define LOCTEXT_NAMESPACE "FAWSLambdaModule"

void* FAWSLambdaModule::AWSLambdaLibraryHandle = nullptr;

void FAWSLambdaModule::StartupModule()
{
#if PLATFORM_WINDOWS && PLATFORM_64BITS
	LOG_NORMAL("Starting Lambda Module...");

	const FString BaseDir = IPluginManager::Get().FindPlugin("GameLiftClientSDK")->GetBaseDir();
	LOG_NORMAL(FString::Printf(TEXT("Base directory is %s"), *BaseDir));

	const FString ThirdPartyDir = FPaths::Combine(*BaseDir, TEXT("ThirdParty"), TEXT("GameLiftClientSDK"), TEXT("Win64"));
	LOG_NORMAL(FString::Printf(TEXT("ThirdParty directory is %s"), *ThirdPartyDir));

	static const FString AWSLambdaDLLName = "aws-cpp-sdk-lambda";
	const bool bDependencyLoaded = LoadDependency(ThirdPartyDir, AWSLambdaDLLName, AWSLambdaLibraryHandle);
	if (bDependencyLoaded == false)
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("Name"), FText::FromString(AWSLambdaDLLName));
		FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("LoadDependencyError", "Failed to load {Name}. Plugin will not be functional"), Arguments));
		FreeDependency(AWSLambdaLibraryHandle);
	}
#endif
}

void FAWSLambdaModule::ShutdownModule()
{
	FreeDependency(AWSLambdaLibraryHandle);
	LOG_NORMAL("Shutting down CognitoIdentity Module...");
}

bool FAWSLambdaModule::LoadDependency(const FString& Dir, const FString& Name, void*& Handle)
{
	FString Lib = Name + TEXT(".") + FPlatformProcess::GetModuleExtension();
	FString Path = Dir.IsEmpty() ? *Lib : FPaths::Combine(*Dir, *Lib);

	Handle = FPlatformProcess::GetDllHandle(*Path);

	if (Handle == nullptr)
	{
		LOG_ERROR(FString::Printf(TEXT("Dependency %s failed to load in directory %s"), *Lib, *Dir));
		return false;
	}

	LOG_NORMAL(FString::Printf(TEXT("Dependency %s successfully loaded from directory %s"), *Lib, *Dir));
	return true;
}

void FAWSLambdaModule::FreeDependency(void*& Handle)
{
#if !PLATFORM_LINUX
	if (Handle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(Handle);
		Handle = nullptr;
	}
#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAWSLambdaModule, AWSLambda);
