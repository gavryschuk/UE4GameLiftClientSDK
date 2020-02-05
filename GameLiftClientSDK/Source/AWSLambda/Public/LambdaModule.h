#pragma once

#include "ModuleManager.h"

class FLambdaModule : public IModuleInterface
{
public:
	void StartupModule();
	void ShutdownModule();

private:
	static void* LambdaLibraryHandle;
	static bool LoadDependency(const FString& Dir, const FString& Name, void*& Handle);
	static void FreeDependency(void*& Handle);
};