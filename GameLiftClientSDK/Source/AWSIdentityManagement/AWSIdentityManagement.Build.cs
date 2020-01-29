using UnrealBuildTool;
using System.IO;

public class AWSIdentityManagement : ModuleRules
{
	public AWSIdentityManagement(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

		PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "CoreUObject", "InputCore", "Projects" });
		
		// This is required to fix a warning for Unreal Engine 4.21 and later
        PrivatePCHHeaderFile = "Private/AWSIdentityManagementPrivatePCH.h";

		string BaseDirectory = System.IO.Path.GetFullPath(System.IO.Path.Combine(ModuleDirectory, "..", ".."));
        string ThirdPartyPath = System.IO.Path.Combine(BaseDirectory, "ThirdParty", "GameLiftClientSDK", Target.Platform.ToString());
        bool bIsThirdPartyPathValid = System.IO.Directory.Exists(ThirdPartyPath);

		if (bIsThirdPartyPathValid)
		{
			PublicLibraryPaths.Add(ThirdPartyPath);

			string AWSIdentityManagementLibFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-identity-management.lib");
			if (File.Exists(AWSIdentityManagementLibFile))
			{
				PublicAdditionalLibraries.Add(AWSIdentityManagementLibFile);
			}
			else
			{
				throw new BuildException("aws-cpp-sdk-identity-management.lib not found. Expected in this location: " + AWSIdentityManagementLibFile);
			}

			string AWSIdentityManagementDLLFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-identity-management.dll");
			if (File.Exists(AWSIdentityManagementDLLFile))
			{
                PublicDelayLoadDLLs.Add("aws-cpp-sdk-identity-management.dll");
                RuntimeDependencies.Add(AWSIdentityManagementDLLFile);
			}
			else
			{
				throw new BuildException("aws-cpp-sdk-lambda.dll not found. Expected in this location: " + AWSIdentityManagementDLLFile);
			}
		}
	}
}
