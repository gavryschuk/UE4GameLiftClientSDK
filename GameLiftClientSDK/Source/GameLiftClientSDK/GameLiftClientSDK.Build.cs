// Created by YetiTech Studios.

using UnrealBuildTool;
using System.IO;

public class GameLiftClientSDK : ModuleRules
{
	public GameLiftClientSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDefinitions.Add("USE_IMPORT_EXPORT");
        PublicDefinitions.Add("USE_WINDOWS_DLL_SEMANTICS");

        bEnableExceptions = true;

        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "CoreUObject", "InputCore", "Projects", "AWSCore", "AWSCognitoIdentity", "AWSIdentityManagement", "AWSLambda" });

        string BaseDirectory = System.IO.Path.GetFullPath(System.IO.Path.Combine(ModuleDirectory, "..", ".."));
        string ThirdPartyPath = System.IO.Path.Combine(BaseDirectory, "ThirdParty", "GameLiftClientSDK", Target.Platform.ToString());
        bool bIsThirdPartyPathValid = System.IO.Directory.Exists(ThirdPartyPath);
		
		if (bIsThirdPartyPathValid)
		{
            if (Target.Type != TargetRules.TargetType.Server)
            {
                PublicDefinitions.Add("WITH_GAMELIFTCLIENTSDK=1");
                PublicLibraryPaths.Add(ThirdPartyPath);

                string GameLiftLibFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-gamelift.lib");
                if (File.Exists(GameLiftLibFile))
                {
                    PublicAdditionalLibraries.Add(GameLiftLibFile);
                }
                else
                {
                    throw new BuildException("aws-cpp-sdk-gamelift.lib not found. Expected in this location: " + GameLiftLibFile);
                }

                string GameLiftDLLFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-gamelift.dll");
                if (File.Exists(GameLiftDLLFile))
                {
                    PublicDelayLoadDLLs.Add("aws-cpp-sdk-gamelift.dll");
                    RuntimeDependencies.Add(GameLiftDLLFile);
                }
                else
                {
                    throw new BuildException("aws-cpp-sdk-gamelift.dll not found. Expected in this location: " + GameLiftDLLFile);
                }

                string BinariesDirectory = System.IO.Path.Combine(BaseDirectory, "Binaries", Target.Platform.ToString());
                if (!Directory.Exists(BinariesDirectory))
                {
                    Directory.CreateDirectory(BinariesDirectory);
                }
                if (File.Exists(System.IO.Path.Combine(BinariesDirectory, "aws-cpp-sdk-gamelift.dll")) == false)
                {
                    File.Copy(System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-gamelift.dll"), System.IO.Path.Combine(BinariesDirectory, "aws-cpp-sdk-gamelift.dll"));
                }
            }
            else
            {
                PublicDefinitions.Add("WITH_GAMELIFTCLIENTSDK=0");
            }
        }
		else
		{
            PublicDefinitions.Add("WITH_GAMELIFTCLIENTSDK=0");
        }
	}
}
