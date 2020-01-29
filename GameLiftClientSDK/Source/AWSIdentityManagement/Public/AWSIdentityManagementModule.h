#pragma once

#include "ModuleManager.h"

class FAWSIdentityManagementModule : public IModuleInterface
{
public:
	void StartupModule();
	void ShutdownModule();

private:
	static void* AWSstsLibraryHandle;
	static void* AWSIdentityManagementLibraryHandle;
	static bool LoadDependency(const FString& Dir, const FString& Name, void*& Handle);
	static void FreeDependency(void*& Handle);
};