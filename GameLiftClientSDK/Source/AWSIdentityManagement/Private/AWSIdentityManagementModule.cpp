#include "AWSIdentityManagementModulePrivatePCH.h"
#include "AWSIdentityManagementModule.h"
#include "GameLiftClientSDK/Public/GameLiftClientGlobals.h"
#include "IPluginManager.h"

#define LOCTEXT_NAMESPACE "FAWSIdentityManagementModule"
void* FAWSIdentityManagementModule::AWSstsLibraryHandle = nullptr;
void* FAWSIdentityManagementModule::AWSIdentityManagementLibraryHandle = nullptr;

void FAWSIdentityManagementModule::StartupModule()
{
#if PLATFORM_WINDOWS && PLATFORM_64BITS
	LOG_NORMAL("Starting CognitoIdentity Module...");

	const FString BaseDir = IPluginManager::Get().FindPlugin("GameLiftClientSDK")->GetBaseDir();
	LOG_NORMAL(FString::Printf(TEXT("Base directory is %s"), *BaseDir));

	const FString ThirdPartyDir = FPaths::Combine(*BaseDir, TEXT("ThirdParty"), TEXT("GameLiftClientSDK"), TEXT("Win64"));
	LOG_NORMAL(FString::Printf(TEXT("ThirdParty directory is %s"), *ThirdPartyDir));

	static const FString AWSstsDLLName = "aws-cpp-sdk-sts";
	static const FString AWSIdentityManagementDLLName = "aws-cpp-sdk-identity-management";

	const bool bStsDependencyLoaded = LoadDependency(ThirdPartyDir, AWSstsDLLName, AWSIdentityManagementLibraryHandle);
	if (bStsDependencyLoaded == false)
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("Name"), FText::FromString(AWSstsDLLName));
		FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("LoadDependencyError", "Failed to load {Name}. Plugin will not be functional"), Arguments));
		FreeDependency(AWSIdentityManagementLibraryHandle);
}

	const bool bIdentityDependencyLoaded = LoadDependency(ThirdPartyDir, AWSIdentityManagementDLLName, AWSIdentityManagementLibraryHandle);
	if (bIdentityDependencyLoaded == false)
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("Name"), FText::FromString(AWSIdentityManagementDLLName));
		FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("LoadDependencyError", "Failed to load {Name}. Plugin will not be functional"), Arguments));
		FreeDependency(AWSIdentityManagementLibraryHandle);
	}
#endif
}

void FAWSIdentityManagementModule::ShutdownModule()
{
	FreeDependency(AWSstsLibraryHandle);
	FreeDependency(AWSIdentityManagementLibraryHandle);
	LOG_NORMAL("Shutting down CognitoIdentity Module...");
}

bool FAWSIdentityManagementModule::LoadDependency(const FString& Dir, const FString& Name, void*& Handle)
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

void FAWSIdentityManagementModule::FreeDependency(void*& Handle)
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

IMPLEMENT_MODULE(FAWSIdentityManagementModule, AWSIdentityManagement);
