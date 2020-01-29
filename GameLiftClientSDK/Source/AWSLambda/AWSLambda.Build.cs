using UnrealBuildTool;
using System.IO;

public class AWSLambda : ModuleRules
{
	public AWSLambda(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

		PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "CoreUObject", "InputCore", "Projects" });
		
		// This is required to fix a warning for Unreal Engine 4.21 and later
        PrivatePCHHeaderFile = "Private/AWSLambdaPrivatePCH.h";

		string BaseDirectory = System.IO.Path.GetFullPath(System.IO.Path.Combine(ModuleDirectory, "..", ".."));
        string ThirdPartyPath = System.IO.Path.Combine(BaseDirectory, "ThirdParty", "GameLiftClientSDK", Target.Platform.ToString());
        bool bIsThirdPartyPathValid = System.IO.Directory.Exists(ThirdPartyPath);

		if (bIsThirdPartyPathValid)
		{
			PublicLibraryPaths.Add(ThirdPartyPath);

			string AWSLambdaLibFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-lambda.lib");
			if (File.Exists(AWSLambdaLibFile))
			{
				PublicAdditionalLibraries.Add(AWSLambdaLibFile);
			}
			else
			{
				throw new BuildException("aws-cpp-sdk-lambda.lib not found. Expected in this location: " + AWSLambdaLibFile);
			}

			string AWSLambdaDLLFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-lambda.dll");
			if (File.Exists(AWSLambdaDLLFile))
			{
                PublicDelayLoadDLLs.Add("aws-cpp-sdk-lambda.dll");
                RuntimeDependencies.Add(AWSLambdaDLLFile);
			}
			else
			{
				throw new BuildException("aws-cpp-sdk-lambda.dll not found. Expected in this location: " + AWSLambdaDLLFile);
			}
		}
	}
}
