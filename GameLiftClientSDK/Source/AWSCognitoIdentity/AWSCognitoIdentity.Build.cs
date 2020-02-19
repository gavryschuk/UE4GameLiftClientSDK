using UnrealBuildTool;
using System.IO;

public class AWSCognitoIdentity : ModuleRules
{
	public AWSCognitoIdentity(ReadOnlyTargetRules Target) : base(Target)
	{
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "CoreUObject", "InputCore", "Projects" });

        PublicDefinitions.Add("USE_IMPORT_EXPORT");
        PublicDefinitions.Add("USE_WINDOWS_DLL_SEMANTICS");

        // This is required to fix a warning for Unreal Engine 4.21 and later
        PrivatePCHHeaderFile = "Private/AWSCognitoIdentityPrivatePCH.h";

        bEnableExceptions = true;

        string BaseDirectory = System.IO.Path.GetFullPath(System.IO.Path.Combine(ModuleDirectory, "..", ".."));
        string ThirdPartyPath = System.IO.Path.Combine(BaseDirectory, "ThirdParty", "GameLiftClientSDK", Target.Platform.ToString());
        bool bIsThirdPartyPathValid = System.IO.Directory.Exists(ThirdPartyPath);

		if (bIsThirdPartyPathValid)
		{
			PublicLibraryPaths.Add(ThirdPartyPath);

			string AWSCognitoLibFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-cognito-identity.lib");
			if (File.Exists(AWSCognitoLibFile))
			{
				PublicAdditionalLibraries.Add(AWSCognitoLibFile);
			}
			else
			{
				throw new BuildException("aws-cpp-sdk-cognito-identity.lib not found. Expected in this location: " + AWSCognitoLibFile);
			}

			string AWSCognitoDLLFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-cognito-identity.dll");
			if (File.Exists(AWSCognitoDLLFile))
			{
                PublicDelayLoadDLLs.Add("aws-cpp-sdk-cognito-identity.dll");
                RuntimeDependencies.Add(AWSCognitoDLLFile);
            }
			else
			{
				throw new BuildException("aws-cpp-sdk-cognito-identity.dll not found. Expected in this location: " + AWSCognitoDLLFile);
			}

            string BinariesDirectory = System.IO.Path.Combine(BaseDirectory, "Binaries", Target.Platform.ToString());
            if (!Directory.Exists(BinariesDirectory))
            {
                Directory.CreateDirectory(BinariesDirectory);
            }
            if (File.Exists(System.IO.Path.Combine(BinariesDirectory, "aws-cpp-sdk-cognito-identity.dll")) == false)
            {
                File.Copy(System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-cognito-identity.dll"), System.IO.Path.Combine(BinariesDirectory, "aws-cpp-sdk-cognito-identity.dll"));
            }
        }
	}
}
