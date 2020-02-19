#pragma once

#include "ModuleManager.h"

class FAWSLogsModule : public IModuleInterface
{
public:
	void StartupModule();
	void ShutdownModule();

private:
	static void* AWSLogsLibraryHandle;
	static bool LoadDependency(const FString& Dir, const FString& Name, void*& Handle);
	static void FreeDependency(void*& Handle);
};