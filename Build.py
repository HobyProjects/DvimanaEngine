#!/usr/bin/env python

import os as system_os
import sys
import json
import argparse

def cmd(command):
    try:
        print("Executing command: " + command)
        system_os.system(command)
    except Exception as e:
        print(e)
        print("Failed to execute command: " + command)
        sys.exit(1)


class ProjectExternalLibs:
    def __init__(self, name, dir):
        self.name = name
        self.src_dir = f"Vendors/{dir}"
        self.build_dir = f"build/config/{name}"
        self.prefix_dir = f"build/packages/{name}"

    def __str__(self):
        return f"Name: {self.name}\nDir: {self.dir}\nBuild Dir: {self.build_dir}\nPrefix: {self.prefix}\n"
    

def get_base_configure_preset():
    return {
        "name": "common-base",
        "hidden": True,
        "binaryDir": "${sourceDir}/out/build/${presetName}",
        "installDir": "${sourceDir}/out/install/${presetName}"
    }


def get_os_base_configure_preset(os: str, inherits: str, cache_variables: list):
    result = {
        "name": os.lower() + "-base",
        "hidden": True,
        "inherits": inherits,
        "generator": "Ninja" if os == "Windows" else "Unix Makefiles",
        "condition": {
            "type": "equals",
            "lhs": "${hostSystemName}",
            "rhs": "Darwin" if os == "macOS" else os
        }
    }
    if os == 'Windows':
        pass # add Windows-specific cache variables here
    else:
        result["cacheVariables"] = {
            "CMAKE_BUILD_TYPE_INIT": f"{cache_variables[0]}",
            "CMAKE_INSTALL_PREFIX": f"{cache_variables[1]}",
            "CMAKE_PREFIX_PATH": f"{cache_variables[2]}"
        }
        result["vendor"] = {
            "microsoft.com/VisualStudioSettings/CMake/1.0": {"hostOS": [os]},
            "microsoft.com/VisualStudioRemoteSettings/CMake/1.0": {"sourceDir": "$env{HOME}/.vs/$ms{projectDirName}"}
        }
    return result


def get_os_preset(os: str, inherits: str, arch: str, conf: str):
    result = {
        "name": f"{os.lower()}-{arch}-{conf.lower()}",
        "inherits": inherits,
        "displayName": f"{arch}-{conf}",
        "architecture": {"value": arch, "strategy": "external"},
        "cacheVariables": {
            "CMAKE_BUILD_TYPE_INIT": f"{conf}"
        }
    }
    return result


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Build script for the Dvimana Engine"
    )

    parser.add_argument(
        "--config",
        type=str,
        help="The build configuration to use. The following values are valid:"
            "  - Debug: Build with debugging symbols and no optimizations."
            "  - Release: Build with optimizations but no debugging symbols."
    )

    args = parser.parse_args()
    
    JSON_PREFIX_PATHS = list()
    CMAKE_PREFIX_PATH = list()

    if sys.platform == "win32":
        JSON_PREFIX_PATHS.append(f"{system_os.path.abspath("build/packages")};") 
        CMAKE_PREFIX_PATH.append("build/packages;")
    else:
        JSON_PREFIX_PATHS.append(f"{system_os.path.abspath("build/packages")}:")
        CMAKE_PREFIX_PATH.append("build/packages:")


    PROJECT_EXTERNALS = [
        ProjectExternalLibs("glfw", "glfw"),
        ProjectExternalLibs("spdlog", "spdlog"),
        ProjectExternalLibs("glew", "glew-cmake"),
        ProjectExternalLibs("glm", "glm"),
        ProjectExternalLibs("entt", "entt"),
        ProjectExternalLibs("ImGuiDocking", "imgui_docking"),
        ProjectExternalLibs("StbImage", "stb_image")
    ]

    if args.config != "Debug" and args.config != "Release":
        print(f"-- Invalid build configuration: {args.config}")
        print("-- Valid configurations are: Debug, Release")
        sys.exit(1)


    if args.config == "Release":
        BUILD_TYPE = "Release"
    else:
        BUILD_TYPE = "Debug"


    for external_module in PROJECT_EXTERNALS:
        if not system_os.path.exists(external_module.src_dir):
            print(f"-- {external_module.name}:MISSING")
            print("Please run 'git submodule update --init --recursive'")
            sys.exit(1)
        else:
            print(f"-- {external_module.name}:FOUND")
            if sys.platform == "win32":
                JSON_PREFIX_PATHS.append(f"{system_os.path.abspath(external_module.prefix_dir)};")
                CMAKE_PREFIX_PATH.append(f"{external_module.prefix_dir};")
            else:
                JSON_PREFIX_PATHS.append(f"{system_os.path.abspath(external_module.prefix_dir)}:")
                CMAKE_PREFIX_PATH.append(f"{external_module.prefix_dir}:")


    CMAKE_PREFIX_PATHS_STRING = "".join(CMAKE_PREFIX_PATH)
    JSON_PREFIX_PATHS_STRING = "".join(JSON_PREFIX_PATHS)

    CMAKE_CACHE_VARIABLES = f"-DCMAKE_BUILD_TYPE_INIT={BUILD_TYPE} -DCMAKE_INSTALL_PREFIX={CMAKE_PREFIX_PATHS_STRING} -DCMAKE_PREFIX_PATH={CMAKE_PREFIX_PATHS_STRING}"
    JSON_CACHE_VARIABLES = [ BUILD_TYPE, JSON_PREFIX_PATHS_STRING, JSON_PREFIX_PATHS_STRING ]


    for external_module in PROJECT_EXTERNALS:
        cmd(f"cmake {CMAKE_CACHE_VARIABLES} -S {external_module.src_dir} -B {external_module.build_dir}")
        cmd(f"cmake --build {external_module.build_dir} --config {BUILD_TYPE}")
        cmd(f"cmake --install {external_module.build_dir} --config {BUILD_TYPE} --prefix {external_module.prefix_dir}")

    # ==============================================================================================================================================

    configure_presets = []
    build_presets = []
    test_presets = []

    base_configure_preset = get_base_configure_preset()
    base_build_preset = {
        'name': 'common-base',
        'hidden': True,
        'jobs': 1,
        'cleanFirst': False
    }
    base_test_preset = {
        "name": "common-base",
        "description": "Basic shared test settings",
        "hidden": True,
        "execution": {
            "noTestsAction": "error",
            "stopOnFailure": False
        },
        "output": {
            "outputOnFailure": True
        }
    }

    configure_presets.append(base_configure_preset)
    build_presets.append(base_build_preset)
    test_presets.append(base_test_preset)

    build_type_debug = True if BUILD_TYPE == "Debug" else False
    build_type_release = True if BUILD_TYPE == "Release" else False

    os_names = ["Linux" , "Windows", "macOS"]
    configs = [ "Debug", "Release"]

    for _os in os_names:
        os_base_configure_preset = get_os_base_configure_preset(_os, base_configure_preset["name"], JSON_CACHE_VARIABLES)
        configure_presets.append(os_base_configure_preset)
        for conf in configs:
            configure_presets.append(get_os_preset(_os, os_base_configure_preset["name"], "x64", conf))
            configure_presets.append(get_os_preset(_os, os_base_configure_preset["name"], "x86", conf))
                
        build_presets.append({"name": os_base_configure_preset["name"], "inherits": base_build_preset["name"], "hidden": True})
        test_presets.append({"name": os_base_configure_preset["name"], "inherits": base_test_preset["name"], "hidden": True})

    for conf_preset in configure_presets:
        if conf_preset.get('hidden', False) == True:
            continue
        build_presets.append({
            "name": conf_preset["name"],
            "inherits": conf_preset["inherits"],
            "displayName": conf_preset["displayName"],
            "configurePreset": conf_preset["name"],
        })
        test_presets.append({
            "name": conf_preset["name"],
            "inherits": conf_preset["inherits"],
            "displayName": conf_preset["displayName"],
            "configurePreset": conf_preset["name"],
        })

    root_presets = {
        "version": 3,
        "configurePresets": configure_presets,
        "buildPresets": build_presets,
        "testPresets": test_presets
    }

    if system_os.path.exists("Src/CMakePresets.json"):
        system_os.remove("Src/CMakePresets.json")

    with open("Src/CMakePresets.json", 'a+') as f:
        json.dump(root_presets, f, indent=2)