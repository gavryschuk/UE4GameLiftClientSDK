using UnrealBuildTool;
using System.IO;

public class AWSIdentityManagement : ModuleRules
{
	public AWSIdentityManagement(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

		PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "CoreUObject", "InputCore", "Projects" });

        PublicDefinitions.Add("USE_IMPORT_EXPORT");
        PublicDefinitions.Add("USE_WINDOWS_DLL_SEMANTICS");

        // This is required to fix a warning for Unreal Engine 4.21 and later
        PrivatePCHHeaderFile = "Private/AWSIdentityManagementPrivatePCH.h";

        bEnableExceptions = true;

        string BaseDirectory = System.IO.Path.GetFullPath(System.IO.Path.Combine(ModuleDirectory, "..", ".."));
        string ThirdPartyPath = System.IO.Path.Combine(BaseDirectory, "ThirdParty", "GameLiftClientSDK", Target.Platform.ToString());
        bool bIsThirdPartyPathValid = System.IO.Directory.Exists(ThirdPartyPath);

		if (bIsThirdPartyPathValid)
		{
            if (Target.Type != TargetRules.TargetType.Server)
            {
                PublicLibraryPaths.Add(ThirdPartyPath);

                string AWSstsLibFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-sts.lib");
                if (File.Exists(AWSstsLibFile))
                {
                    PublicAdditionalLibraries.Add(AWSstsLibFile);
                }
                else
                {
                    throw new BuildException("aws-cpp-sdk-sts.lib not found. Expected in this location: " + AWSstsLibFile);
                }

                string AWSstsDLLFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-sts.dll");
                if (File.Exists(AWSstsDLLFile))
                {
                    PublicDelayLoadDLLs.Add("aws-cpp-sdk-sts.dll");
                    RuntimeDependencies.Add(AWSstsDLLFile);
                }
                else
                {
                    throw new BuildException("aws-cpp-sdk-sts.dll not found. Expected in this location: " + AWSstsDLLFile);
                }

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
                    throw new BuildException("aws-cpp-sdk-identity-management.dll not found. Expected in this location: " + AWSIdentityManagementDLLFile);
                }

                string BinariesDirectory = System.IO.Path.Combine(BaseDirectory, "Binaries", Target.Platform.ToString());
                if (!Directory.Exists(BinariesDirectory))
                {
                    Directory.CreateDirectory(BinariesDirectory);
                }
                if (File.Exists(System.IO.Path.Combine(BinariesDirectory, "aws-cpp-sdk-sts.dll")) == false)
                {
                    File.Copy(System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-sts.dll"), System.IO.Path.Combine(BinariesDirectory, "aws-cpp-sdk-sts.dll"));
                }
                if (File.Exists(System.IO.Path.Combine(BinariesDirectory, "aws-cpp-sdk-identity-management.dll")) == false)
                {
                    File.Copy(System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-identity-management.dll"), System.IO.Path.Combine(BinariesDirectory, "aws-cpp-sdk-identity-management.dll"));
                }
            }
        }
	}
}
