using UnrealBuildTool;
using System.IO;

public class Lambda : ModuleRules
{
	public Lambda(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

		PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "CoreUObject", "InputCore", "Projects", "AWSCore", "CognitoIdentity", "AWSIdentityManagement" });
		
		// This is required to fix a warning for Unreal Engine 4.21 and later
        PrivatePCHHeaderFile = "Private/LambdaPrivatePCH.h";

		string BaseDirectory = System.IO.Path.GetFullPath(System.IO.Path.Combine(ModuleDirectory, "..", ".."));
        string ThirdPartyPath = System.IO.Path.Combine(BaseDirectory, "ThirdParty", "GameLiftClientSDK", Target.Platform.ToString());
        bool bIsThirdPartyPathValid = System.IO.Directory.Exists(ThirdPartyPath);

        if (bIsThirdPartyPathValid)
        {
            PublicDefinitions.Add("WITH_AWS_LAMBDA=1");
            PublicLibraryPaths.Add(ThirdPartyPath);

            string LambdaLibFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-lambda.lib");
            if (File.Exists(LambdaLibFile))
            {
                PublicAdditionalLibraries.Add(LambdaLibFile);
            }
            else
            {
                throw new BuildException("aws-cpp-sdk-lambda.lib not found. Expected in this location: " + LambdaLibFile);
            }

            string LambdaDLLFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-lambda.dll");
            if (File.Exists(LambdaDLLFile))
            {
                PublicDelayLoadDLLs.Add("aws-cpp-sdk-lambda.dll");
                RuntimeDependencies.Add(LambdaDLLFile);
            }
            else
            {
                throw new BuildException("aws-cpp-sdk-lambda.dll not found. Expected in this location: " + LambdaDLLFile);
            }
        }
        else
        {
            PublicDefinitions.Add("WITH_AWS_LAMBDA=0");
        }
	}
}
