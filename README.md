# BRANCH Lambda

Current branch has AWSLambda functionalty added to the plugin.
As the highest priority this is needed to manage game sessions for GameLift Server through lambda for security reasons.
You can find the described AWS lambda function here: [gameLiftPlayerSessionLambda](https://github.com/gavryschuk/aws-lambdas/tree/gamelift-get-active-player-session)
Also might be used to call Analytics Data Lambdas for your project.

The class structure and design/architecture is the same as GameLiftClientSDK API.

Very convinient to be used as an addition to existing GameLift Client SDK.
Blueprints examples:

- [General Lambda Object setup and Function Call](https://monosnap.com/file/64PG4xeAg2i16xfC38N5JsyQINleP1)
- [Provide Input params to the function](https://monosnap.com/file/xYstckjvqBolmEVLOkc2WTqv00TXIh)
- [Lambda Error Handler](https://monosnap.com/file/1XfUSfdDGVo4pbABeqguF1waHhw4ff)
- [Lambda Succeed. Parsing needed params](https://monosnap.com/file/gf3WQ19TugcyBLfhQ40MzPmtWkvXTz)
-  In the current example Lambda already searched for the active available game sessions (if there were none->created one). Generated Player session for that game session and returned a result. [Using previously parsed **ip adress** and **port** to connect to GameLift Server](https://monosnap.com/file/2BElF5XMNUEfkIwM0YWFK2WW23n2hx?idx=0&total=1064)


To get general information how to setup plugin -> please, read further...


# GameLift Client SDK for Unreal Engine 4

![](https://pbs.twimg.com/profile_images/674912463456894981/zpsLHeRC.png)![](https://pbs.twimg.com/profile_images/675419785178382336/G8JCcref.png)![](https://www.cloudwards.net/wp-content/uploads/2015/10/AWS-logo.png)

### What is Amazon GameLift?
Amazon GameLift is a managed service for deploying, operating, and scaling dedicated game servers for session-based multiplayer games. Amazon GameLift makes it easy to manage server infrastructure, scale capacity to lower latency and cost, match players into available game sessions, and defend from distributed denial-of-service (DDoS) attacks. You pay for the compute resources and bandwidth your games actually use, without monthly or annual contracts.

Epic Games announced [Amazon GameLift support for Unreal Engine 4](https://www.unrealengine.com/en-US/blog/aws-announces-amazon-gamelift-support-for-unreal-engine) but only the server plugin is available. We have made a client plugin for Unreal Engine 4 including Blueprint support that can Create Game Sessions, Describe Game Sessions and Create Player Sessions.

### Whats Included?

This repository includes source files for the plugin as well as pre-built binaries of Core, GameLift and Cognito Identity from [AWS SDK for C++](https://github.com/aws/aws-sdk-cpp). To know the version of AWS, please check the VersionName in GameLiftClientSDK.uplugin.

### Sounds cool, I'm In! What should I do?
If you are using Blueprint-Only project then open your project and add a dummy C++ class from File->New C++ class. This is required to generate project files.
- Download or clone this repository into your **Project/Plugin** folder.
- Add "GameLiftClientSDK" as a public dependency in your **ProjectName.Build.cs** file
```csharp
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameLiftClientSDK" });
```
- Right click on your project file and select **Generate Visual Studio project files**.
- Open your projects solution file (*.sln) and build your project.
- Now if you start your project and go to **Edit->Plugins** you should see the GameLift Client SDK under **Installed->YetiTech Studios** category.

#### Dealing with building error `C4577`

If your build output throws some `C4577` errors when building your project after implementing the plugin, there is one thing you can do to work around it. Open the file [GameLiftClientSDK.Build.cs](https://github.com/YetiTech-Studios/UE4GameLiftClientSDK/blob/master/GameLiftClientSDK/Source/GameLiftClientSDK/GameLiftClientSDK.Build.cs#L9) located at `YourProject/Plugins/GameLiftClientSDK/Source/GameLiftClientSDK/` and after line 9, add the following code fragment:

```cs
bEnableExceptions = true;
```

The final code would look like this:

```cs
public class GameLiftClientSDK : ModuleRules
{
	public GameLiftClientSDK(ReadOnlyTargetRules Target) : base(Target)
	{
        	bEnableExceptions = true;

		// ...
	}
}
```

Then, save the file and build your game again and probably you'll be fine.

### How to use GameLift Client Plugin
You can use this plugin either in Blueprints or C++. In any method, you must first create the GameLift client before accessing it. This is done inside the **GameLiftObject**. After initializing the **GameLiftObject** you can access GameLift Client functions. **GameLiftObject** can be created from any Blueprint. For the sake of this tutorial we will do all this inside our custom [GameInstance](https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Engine/UGameInstance/index.html) class.
#### Blueprints
- Open your custom GameInstance Blueprint class.
- Then add an **Event Init** node.
- Now connect the init node to **Create Game Lift Object** node. In this node don't forget to type your access key and secret key. See [Managing Access Keys for your AWS Account](http://docs.aws.amazon.com/general/latest/gr/managing-aws-access-keys.html) for more information.
[![Image](https://i.imgur.com/Tu1RYkt.png)](https://yetitechstudios.imgur.com/all/)
- Once the above node is created, you are good to create game sessions, player sessions etc. Here is an example network. **NOTE: This is only an example. Implementation might differ according to your project.**
[![Imgur](https://i.imgur.com/KT6qs7h.png)](https://yetitechstudios.imgur.com/all/)

#### C++
> **4.17 Users:** If you are getting an error message like in the below picture, make sure you select No. This is a bug in 4.17 and was resolved in 4.18.

[![Imgur](https://i.imgur.com/u3ALYqV.png)](https://yetitechstudios.imgur.com/all/)

More information here: https://issues.unrealengine.com/issue/UE-49007

Header file.
```cpp
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ExampleGameInstance.generated.h"


UCLASS()
class GAMELIFTPLUGINTEST_API UExampleGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	class UGameLiftClientObject* GameLiftClientObject;

public:

	virtual void Init() override;

	// Create Game Session ///////////////////////////////////////////////////
	void CreateGameSession();
	UFUNCTION()
	void OnGameCreationSuccess(const FString& GameSessionID);
	UFUNCTION()
	void OnGameCreationFailed(const FString& ErrorMessage);

	// Describe Game Session /////////////////////////////////////////////////
	void DescribeGameSession(const FString& GameSessionID);
	UFUNCTION()
	void OnDescribeGameSessionSuccess(const FString& SessionID, EGameLiftGameSessionStatus SessionState);
	UFUNCTION()
	void OnDescribeGameSessionFailed(const FString& ErrorMessage);

	// Create Player Session /////////////////////////////////////////////////
	void CreatePlayerSession(const FString& GameSessionID, const FString UniquePlayerID);
	UFUNCTION()
	void OnPlayerSessionCreateSuccess(const FString& IPAddress, const FString& Port, const FString& PlayerSessionID);
	UFUNCTION()
	void OnPlayerSessionCreateFail(const FString& ErrorMessage);
};
```

Source file.
```cpp
#include "ExampleGameInstance.h"
#include "Kismet/GameplayStatics.h"
#if WITH_GAMELIFTCLIENTSDK
#include "GameLiftClientSDK/Public/GameLiftClientObject.h"
#include "GameLiftClientSDK/Public/GameLiftClientApi.h"
#endif

void UExampleGameInstance::Init()
{
	Super::Init();
#if WITH_GAMELIFTCLIENTSDK
    // Create the game lift object. This is required before calling any GameLift functions.
	GameLiftClientObject = UGameLiftClientObject::CreateGameLiftObject("Your Access Key", "Your Secret Key");
#endif
}

void UExampleGameInstance::CreateGameSession()
{
#if WITH_GAMELIFTCLIENTSDK
	FGameLiftGameSessionConfig MySessionConfig;
	MySessionConfig.SetAliasID("Your Alias ID");
	MySessionConfig.SetMaxPlayers(10);
	UGameLiftCreateGameSession* MyGameSessionObject = GameLiftClientObject->CreateGameSession(MySessionConfig);
	MyGameSessionObject->OnCreateGameSessionSuccess.AddDynamic(this, &UExampleGameInstance::OnGameCreationSuccess);
	MyGameSessionObject->OnCreateGameSessionFailed.AddDynamic(this, &UExampleGameInstance::OnGameCreationFailed);
	MyGameSessionObject->Activate();
#endif
}

void UExampleGameInstance::OnGameCreationSuccess(const FString& GameSessionID)
{
	DescribeGameSession(GameSessionID);
}

void UExampleGameInstance::OnGameCreationFailed(const FString& ErrorMessage)
{
#if WITH_GAMELIFTCLIENTSDK
	// Do stuff...
#endif
}

void UExampleGameInstance::DescribeGameSession(const FString& GameSessionID)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribeGameSession* MyDescribeGameSessionObject = GameLiftClientObject->DescribeGameSession(GameSessionID);
	MyDescribeGameSessionObject->OnDescribeGameSessionStateSuccess.AddDynamic(this, &UExampleGameInstance::OnDescribeGameSessionSuccess);
	MyDescribeGameSessionObject->OnDescribeGameSessionStateFailed.AddDynamic(this, &UExampleGameInstance::OnDescribeGameSessionFailed);
	MyDescribeGameSessionObject->Activate();
#endif
}

void UExampleGameInstance::OnDescribeGameSessionSuccess(const FString& SessionID, EGameLiftGameSessionStatus SessionState)
{
	// Player sessions can only be created on ACTIVE instance.
	if (SessionState == EGameLiftGameSessionStatus::STATUS_Active)
	{
		CreatePlayerSession(SessionID, "Your Unique Player ID");
	}
}

void UExampleGameInstance::OnDescribeGameSessionFailed(const FString& ErrorMessage)
{
#if WITH_GAMELIFTCLIENTSDK
	// Do stuff...
#endif
}

void UExampleGameInstance::CreatePlayerSession(const FString& GameSessionID, const FString UniquePlayerID)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftCreatePlayerSession* MyCreatePlayerSessionObject = GameLiftClientObject->CreatePlayerSession(GameSessionID, UniquePlayerID);
	MyCreatePlayerSessionObject->OnCreatePlayerSessionSuccess.AddDynamic(this, &UExampleGameInstance::OnPlayerSessionCreateSuccess);
	MyCreatePlayerSessionObject->OnCreatePlayerSessionFailed.AddDynamic(this, &UExampleGameInstance::OnPlayerSessionCreateFail);
	MyCreatePlayerSessionObject->Activate();
#endif
}

void UExampleGameInstance::OnPlayerSessionCreateSuccess(const FString& IPAddress, const FString& Port, const FString& PlayerSessionID)
{
#if WITH_GAMELIFTCLIENTSDK
	const FString TravelURL = IPAddress + ":" + Port;
	UGameplayStatics::GetPlayerController(this, 0)->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
#endif
}

void UExampleGameInstance::OnPlayerSessionCreateFail(const FString& ErrorMessage)
{
#if WITH_GAMELIFTCLIENTSDK
	// Do stuff...
#endif
}
```
