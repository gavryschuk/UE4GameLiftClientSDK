// AMAZON CONFIDENTIAL

/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

using UnrealBuildTool;
using System.IO;

public class AWSLogs : ModuleRules
{
    public AWSLogs(ReadOnlyTargetRules Target) : base (Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDefinitions.Add("USE_IMPORT_EXPORT");
        PublicDefinitions.Add("USE_WINDOWS_DLL_SEMANTICS");

        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Projects", "Engine", "AWSCore" });

        // This is required to fix a warning for Unreal Engine 4.21 and later
        PrivatePCHHeaderFile = "Private/AWSLogsModulePrivatePCH.h";

        bEnableExceptions = true;

        string BaseDirectory = System.IO.Path.GetFullPath(System.IO.Path.Combine(ModuleDirectory, "..", ".."));
        string ThirdPartyPath = System.IO.Path.Combine(BaseDirectory, "ThirdParty", "GameLiftClientSDK", Target.Platform.ToString());
        bool bIsThirdPartyPathValid = System.IO.Directory.Exists(ThirdPartyPath);

        if (bIsThirdPartyPathValid)
        {
            PublicDefinitions.Add("WITH_AWS_LOGS=1");
            PublicLibraryPaths.Add(ThirdPartyPath);

            string AWSLogsLibFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-logs.lib");
            if (File.Exists(AWSLogsLibFile))
            {
                PublicAdditionalLibraries.Add(AWSLogsLibFile);
            }
            else
            {
                throw new BuildException("aws-cpp-sdk-logs.lib not found. Expected in this location: " + AWSLogsLibFile);
            }

            string AWSLogsDLLFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-logs.dll");
            if (File.Exists(AWSLogsDLLFile))
            {
                PublicDelayLoadDLLs.Add("aws-cpp-sdk-logs.dll");
                RuntimeDependencies.Add(AWSLogsDLLFile);
            }
            else
            {
                throw new BuildException("aws-cpp-sdk-logs.dll not found. Expected in this location: " + AWSLogsDLLFile);
            }

            string BinariesDirectory = System.IO.Path.Combine(BaseDirectory, "Binaries", Target.Platform.ToString());
            if (!Directory.Exists(BinariesDirectory))
            {
                Directory.CreateDirectory(BinariesDirectory);
            }
            if (File.Exists(System.IO.Path.Combine(BinariesDirectory, "aws-cpp-sdk-logs.dll")) == false)
            {
                File.Copy(System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-logs.dll"), System.IO.Path.Combine(BinariesDirectory, "aws-cpp-sdk-logs.dll"));
            }
        }
        else
        {
            PublicDefinitions.Add("WITH_AWS_LOGS=0");
        }
    }
}
