#include "AWSCognitoIdentityModulePrivatePCH.h"
#include "AWSCognitoIdentityModule.h"
#include "GameLiftClientSDK/Public/GameLiftClientGlobals.h"
#include "IPluginManager.h"

#define LOCTEXT_NAMESPACE "FAWSCognitoIdentityModule"

void* FAWSCognitoIdentityModule::AWSCognitoIdentityLibraryHandle = nullptr;

void FAWSCognitoIdentityModule::StartupModule()
{
#if PLATFORM_WINDOWS && PLATFORM_64BITS
	LOG_NORMAL("Starting CognitoIdentity Module...");

	const FString BaseDir = IPluginManager::Get().FindPlugin("GameLiftClientSDK")->GetBaseDir();
	LOG_NORMAL(FString::Printf(TEXT("Base directory is %s"), *BaseDir));

	const FString ThirdPartyDir = FPaths::Combine(*BaseDir, TEXT("ThirdParty"), TEXT("GameLiftClientSDK"), TEXT("Win64"));
	LOG_NORMAL(FString::Printf(TEXT("ThirdParty directory is %s"), *ThirdPartyDir));

	static const FString AWSCognitoIdentityDLLName = "aws-cpp-sdk-cognito-identity";
	const bool bDependencyLoaded = LoadDependency(ThirdPartyDir, AWSCognitoIdentityDLLName, AWSCognitoIdentityLibraryHandle);
	if (bDependencyLoaded == false)
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("Name"), FText::FromString(AWSCognitoIdentityDLLName));
		FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("LoadDependencyError", "Failed to load {Name}. Plugin will not be functional"), Arguments));
		FreeDependency(AWSCognitoIdentityLibraryHandle);
	}
#endif
}

void FAWSCognitoIdentityModule::ShutdownModule()
{
	FreeDependency(AWSCognitoIdentityLibraryHandle);
	LOG_NORMAL("Shutting down CognitoIdentity Module...");
}

bool FAWSCognitoIdentityModule::LoadDependency(const FString& Dir, const FString& Name, void*& Handle)
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

void FAWSCognitoIdentityModule::FreeDependency(void*& Handle)
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

IMPLEMENT_MODULE(FAWSCognitoIdentityModule, AWSCognitoIdentity);
