#pragma once

#include "ModuleManager.h"

class FAWSCognitoIdentityModule : public IModuleInterface
{
public:
	void StartupModule();
	void ShutdownModule();

private:
	static void* AWSCognitoIdentityLibraryHandle;
	static bool LoadDependency(const FString& Dir, const FString& Name, void*& Handle);
	static void FreeDependency(void*& Handle);
};