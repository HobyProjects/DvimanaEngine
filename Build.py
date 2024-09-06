#!/usr/bin/env python

import os
import sys
import argparse

parser = argparse.ArgumentParser(
    description="Build script for the Dvimana Engine"
)

parser.add_argument(
    "--config",
    type=str,
    help="The build configuration to use. The following values are valid:\n"
         "  - Debug: Build with debugging symbols and no optimizations.\n"
         "  - Release: Build with optimizations but no debugging symbols.\n"
)

args = parser.parse_args()

def cmd(command):
    try:
        print("Executing command: " + command)
        os.system(command)
    except Exception as e:
        print(e)
        print("Failed to execute command: " + command)
        sys.exit(1)


BUILD_PATH = "build" 
CONFIG_PATH = "build/config"

PREFIX_PATHS = list()
if sys.platform == "win32":
    PREFIX_PATHS.append("build/packages;") 
else:
    PREFIX_PATHS.append("build/packages:")

SUBMODULES_PATH = "Vendors"
CMAKE_MODULES_PATH = "CMake"  
EXTERNAL_SUBMODULES = [ "glfw", "spdlog", "glew-cmake", "glm", "entt" ]
PROJECT_SUBMODULES = [ "imgui_docking", "stb_image" ]

def verify_submodules():
    for ex_submodule in EXTERNAL_SUBMODULES:
        if not os.path.isdir(f"{SUBMODULES_PATH}/{ex_submodule}"):
            print(f"Missing submodule: {ex_submodule}")
            print("Please run 'git submodule update --init --recursive'")
            sys.exit(1)
        else:
            if sys.platform == "win32":
                PREFIX_PATHS.append(f"build/packages/{ex_submodule};") 
            else:
                PREFIX_PATHS.append(f"build/packages/{ex_submodule}:")

    for proj_submodule in PROJECT_SUBMODULES:
        if not os.path.isdir(f"{SUBMODULES_PATH}/{proj_submodule}"):
            print(f"Missing submodule: {proj_submodule}")
            print("Please run 'git submodule update --init --recursive'")
            sys.exit(1)
        else:
            if sys.platform == "win32":
                PREFIX_PATHS.append(f"build/packages/{proj_submodule};") 
            else:
                PREFIX_PATHS.append(f"build/packages/{proj_submodule}:")


verify_submodules()
if args.config != "Debug" and args.config != "Release":
    print("Invalid build configuration: " + args.config)
    print("Valid configurations are: Debug, Release")
    sys.exit(1)


if args.config == "Release":
    BUILD_TYPE = "Release"
else:
    BUILD_TYPE = "Debug"


print("Building with configuration: " + BUILD_TYPE)

PREDIX_PATHS_STR = "".join(PREFIX_PATHS)
print("Prefix paths: " + PREDIX_PATHS_STR)

CMAKE_CACHE_VARIABLES = f"""
-DCMAKE_BUILD_TYPE_INIT={BUILD_TYPE} 
-DCMAKE_INSTALL_PREFIX={PREDIX_PATHS_STR} 
-DCMAKE_PREFIX_PATH={PREDIX_PATHS_STR}
-DCMAKE_MODULE_PATH={CMAKE_MODULES_PATH}
""".replace("\n", " ").replace("\t", " ")


#======================================================================================================================================================================================

for ex_submodule in EXTERNAL_SUBMODULES:
    cmd(f"cmake {CMAKE_CACHE_VARIABLES} -S Vendors/{ex_submodule} -B build/config/{ex_submodule}")
    cmd(f"cmake --build build/config/{ex_submodule} --config {BUILD_TYPE}")
    cmd(f"cmake --install build/config/{ex_submodule} --config {BUILD_TYPE} --prefix build/packages/{ex_submodule}")

#======================================================================================================================================================================================

for proj_submodule in PROJECT_SUBMODULES:
    cmd(f"cmake {CMAKE_CACHE_VARIABLES} -S Vendors/{proj_submodule} -B build/config/{proj_submodule}")
    cmd(f"cmake --build build/config/{proj_submodule} --config {BUILD_TYPE}")
    cmd(f"cmake --install build/config/{proj_submodule} --config {BUILD_TYPE} --prefix build/packages/{proj_submodule}")

#======================================================================================================================================================================================