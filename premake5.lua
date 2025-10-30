-- GTAAudioMetadataTool Premake5 Configuration

workspace "GTAAudioMetadataTool"
architecture "x64"
configurations { "Debug", "Release" }
startproject "ivam"

-- Global settings
flags { "MultiProcessorCompile" }
systemversion "latest"
buildoptions { "/MP" }
linkoptions { "/INCREMENTAL:NO" }

-- Output directories
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories
IncludeDir = {}
IncludeDir["nlohmann"] = "vendor/nlohmann/include"
IncludeDir["fifo_map"] = "vendor/fifo_map/src"

project "ivam"
kind "ConsoleApp"
language "C++"
cppdialect "C++20"
staticruntime "on"

targetdir("bin/" .. outputdir .. "/%{prj.name}")
objdir("src/obj/" .. outputdir .. "/%{prj.name}")

pchheader "pch.h"
pchsource "src/pch.cpp"

files
{
    "src/**.h",
    "src/**.hh",
    "src/**.hpp",
    "src/**.c",
    "src/**.cc",
    "src/**.cpp"
}

includedirs
{
    "src",
    "%{IncludeDir.nlohmann}",
    "%{IncludeDir.fifo_map}"
}

defines { "_CRT_SECURE_NO_WARNINGS", "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS" }
buildoptions { 
    "/bigobj", 
    "/Zc:__cplusplus",
    "/permissive-",
    "/Zc:preprocessor"
}

filter "configurations:Debug"
defines { "DEBUG", "_DEBUG" }
runtime "Debug"
symbols "on"
optimize "off"
buildoptions { "/JMC" }
linkoptions { "/DEBUG:FASTLINK" }

filter "configurations:Release"
defines { "NDEBUG", "RELEASE" }
runtime "Release"
symbols "off"
optimize "speed"
buildoptions { 
    "/GL",
    "/Gy"
}
linkoptions { 
    "/LTCG",
    "/OPT:REF",
    "/OPT:ICF"
}

-- Custom actions for dependency management
newaction {
    trigger = "setup-deps",
    description = "Setup dependencies (nlohmann/json and fifo_map)",
    execute = function()
        print("Setting up dependencies...")

        -- Create vendor directory
        if not os.isdir("vendor") then
            os.mkdir("vendor")
        end

        -- Clone nlohmann/json
        if not os.isdir("vendor/nlohmann") then
            print("Cloning nlohmann/json...")
            os.execute("git clone --depth 1 --branch v3.11.3 https://github.com/nlohmann/json.git vendor/nlohmann")
            -- Check if directory was created successfully instead of relying on return code
            if os.isdir("vendor/nlohmann") then
                print("nlohmann/json cloned successfully")
            else
                print("Failed to clone nlohmann/json")
                return
            end
        else
            print("nlohmann/json already exists")
        end

        -- Clone fifo_map
        if not os.isdir("vendor/fifo_map") then
            print("Cloning fifo_map...")
            os.execute("git clone --depth 1 https://github.com/nlohmann/fifo_map.git vendor/fifo_map")
            -- Check if directory was created successfully instead of relying on return code
            if os.isdir("vendor/fifo_map") then
                print("fifo_map cloned successfully")
            else
                print("Failed to clone fifo_map")
                return
            end
        else
            print("fifo_map already exists")
        end
        print("Dependencies setup complete!")
    end
}

newaction {
    trigger = "clean-deps",
    description = "Clean dependencies (remove vendor directory)",
    execute = function()
        print("Cleaning dependencies...")
        if os.isdir("vendor") then
            os.rmdir("vendor")
            print("Vendor directory removed")
        else
            print("Vendor directory doesn't exist")
        end
    end
}
