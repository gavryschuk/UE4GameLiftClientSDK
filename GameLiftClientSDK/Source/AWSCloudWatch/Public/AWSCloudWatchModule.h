#pragma once

#include "ModuleManager.h"

class FAWSCloudWatchModule : public IModuleInterface
{
public:
	void StartupModule();
	void ShutdownModule();

private:
	static void* AWSCloudWatchLibraryHandle;
	static bool LoadDependency(const FString& Dir, const FString& Name, void*& Handle);
	static void FreeDependency(void*& Handle);
};