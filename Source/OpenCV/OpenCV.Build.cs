// All Rights Reserved Benjamin David Andersen (SteamyThePunk) 2016
// Based on https://wiki.unrealengine.com/Integrating_OpenCV_Into_Unreal_Engine_4


using UnrealBuildTool;
using System.IO;

public class OpenCV : ModuleRules
{
    private string ModulePath
    {
        get { return Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name)); }
    }
    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

    public OpenCV(TargetInfo Target)
    {
        // Startard Module Dependencies
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "RHI", "RenderCore", "ShaderCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine" });

        PublicIncludePaths.Add("OpenCV/Public");
        PrivateIncludePaths.Add("OpenCV/Private");

        // Start OpenCV linking here!
        bool isLibrarySupported = false;

        // Create OpenCV Path 
        string OpenCVPath = Path.Combine(ThirdPartyPath, "OpenCV");

        // Get Library Path 
        string LibPath = "";
        bool isdebug = Target.Configuration == UnrealTargetConfiguration.Debug && BuildConfiguration.bDebugBuildsActuallyUseDebugCRT;
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            LibPath = Path.Combine(OpenCVPath, "Libraries", "x64");
            isLibrarySupported = true;
        }
        else
        {
            string Err = string.Format("{0} dedicated server is made to depend on {1}. We want to avoid this, please correct module dependencies.", Target.Platform.ToString(), this.ToString()); System.Console.WriteLine(Err);
        }

        if (isLibrarySupported)
        {
            //Add Include path 
            PublicIncludePaths.AddRange(new string[] { Path.Combine(OpenCVPath, "Includes") });

            // Add Library Path 
            PublicLibraryPaths.Add(LibPath);

            // Add Dependencies 
            if (!isdebug)
            {
                //Add Static Libraries
                PublicAdditionalLibraries.Add("opencv_world310.lib");

                //Add Dynamic Libraries
                PublicDelayLoadDLLs.Add("opencv_world310.dll");
            }
            else
            {
                //Add Static Libraries (Debug Version)
                PublicAdditionalLibraries.Add("opencv_world310d.lib");

                //Add Dynamic Libraries (Debug Version)
                PublicDelayLoadDLLs.Add("opencv_world310d.dll");
            }
        }

        Definitions.Add(string.Format("WITH_OPENCV_BINDING={0}", isLibrarySupported ? 1 : 0));
    }
}