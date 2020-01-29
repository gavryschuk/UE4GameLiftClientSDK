#pragma once

#include "ModuleManager.h"

class FAWSLambdaModule : public IModuleInterface
{
public:
	void StartupModule();
	void ShutdownModule();

private:
	static void* AWSLambdaLibraryHandle;
	static bool LoadDependency(const FString& Dir, const FString& Name, void*& Handle);
	static void FreeDependency(void*& Handle);
};