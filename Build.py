#!/usr/bin/env python

import os as system_os
import sys
import json
import argparse

def echo(message):
    print(f"-- [BUILDER]: {message}")

def cmd(command):
    try:
        echo("Executing command: " + command)
        system_os.system(command)
    except Exception as e:
        echo(e)
        echo("Failed to execute command: " + command)
        sys.exit(1)

class ProjectlLibs:
    def __init__(self, name, srcdir, builddir, prefixdir, options):
        self.name = name
        self.source_directory = f"{srcdir}"
        self.build_dir = f"{builddir}"
        self.prefix_dir = f"{prefixdir}"
        self.options = options

    def __str__(self):
        return f"Name: {self.name}\nDir: {self.dir}\nBuild Dir: {self.build_dir}\nPrefix: {self.prefix}\n Options: {self.options}"

#====================================[ CMakePresets Generator Function ]============================================================================================================   

def get_base_configure_preset():
    return {
        "name": "common-base",
        "hidden": True,
        "binaryDir": "${sourceDir}/build",
        "installDir": "${sourceDir}/build"
    }

def get_os_base_configure_preset(os: str, inherits: str, cache_variables: list):
    result = {
        "name": os.lower() + "-base",
        "hidden": True,
        "inherits": inherits,
        "condition": {
            "type": "equals",
            "lhs": "${hostSystemName}",
            "rhs": "Darwin" if os == "macOS" else os
        }
    }
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

def generate_preset(dir: str, preset_cache_variables: list):
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

    os_names = ["Linux" , "Windows", "macOS"]
    configs = [ "Debug", "Release"]
    
    for _os in os_names:
        os_base_configure_preset = get_os_base_configure_preset(_os, base_configure_preset["name"], preset_cache_variables)
        configure_presets.append(os_base_configure_preset)
        
        for conf in configs:
            conf_preset = get_os_preset(_os, os_base_configure_preset["name"], "x64", conf)
            configure_presets.append(conf_preset)
            conf_preset = get_os_preset(_os, os_base_configure_preset["name"], "x86", conf)
            configure_presets.append(conf_preset)
        
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
    if system_os.path.exists(f"{dir}/CMakePresets.json"):
        system_os.remove(f"{dir}/CMakePresets.json")
        echo(f"Deleting Old CMakePresets.json in {dir}")

    with open(f"{dir}/CMakePresets.json", 'a+') as f:
        json.dump(root_presets, f, indent=2)
    echo(f"Created CMakePresets.json in {dir}")


def get_build_configurations(platform: str, build_type: str, projlibs: list[ProjectlLibs]):
    preset_cache_variables = str()
    targets_cache_variables = str()

    for lib in projlibs:
        if platform == "Windows":
            preset_cache_variables += f"{system_os.path.abspath(lib.prefix_dir)};"
            targets_cache_variables += f"{lib.prefix_dir};"
        else:
            preset_cache_variables += f"{system_os.path.abspath(lib.prefix_dir)}:"
            targets_cache_variables += f"{lib.prefix_dir}:"

    config_list = [ preset_cache_variables, targets_cache_variables ]
    return config_list

#======================================================================================================================================================

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
    parser.add_argument(
        "--arch",
        type=str,
        help="The architecture to build for. The following values are valid:"
            "  - x86: Build for x86 architecture."
            "  - x64: Build for x64 architecture."
    )
    parser.add_argument(
        "--target",
        type=str,
        help="The target to build for. The following values are valid:"
            " - external: Build external libraries / executables only"
            " - internal: Build internal libraries / executables only"
            " - executables: Build executables only"
            " - all: Build all libraries / executables"
    )

    args = parser.parse_args()

    if args.config != "Debug" and args.config != "Release":
        echo(f"Invalid build configuration: {args.config}")
        echo("Valid configurations are: Debug, Release")
        echo("")
        echo("The build configuration determines whether the build is done with")
        echo("debugging symbols and no optimizations, or with optimizations but")
        echo("no debugging symbols.")
        sys.exit(1)

    if args.arch != "x86" and args.arch != "x64":
        echo(f"Invalid architecture: {args.arch}")
        echo("Valid architectures are: x86, x64")
        echo("")
        echo("The target architecture determines whether the build is done for")
        echo("x86 or x64 architecture.")
        sys.exit(1)

    if args.target != "external" and args.target != "internal" and args.target != "all":
        echo(f"Invalid target: {args.target}")
        echo("Valid targets are: external, internal, all")
        echo("")
        echo("The target determines what libraries and executables will be built.")
        echo("If 'external' is specified, the build will be done for the external")
        echo("libraries and executables only. If 'internal' is specified, the build")
        echo("will be done for the internal libraries and executables only. If 'all'")
        echo("is specified, the build will be done for all libraries and executables.")
        sys.exit(1)

    TARGETS_BUILD_TYPE = args.config
    TARGETS_BUILD_ARCHITECTURE = args.arch
    BUILD_TARGETS = args.target
    TARGETS_BUILD_SYSTEM_NAME = str()
    if sys.platform == "win32":
        TARGETS_BUILD_SYSTEM_NAME = "Windows"
    elif sys.platform == "darwin":
        TARGETS_BUILD_SYSTEM_NAME = "MacOS"
    else:
        TARGETS_BUILD_SYSTEM_NAME = "Linux"

#======================================================================================================================================================

    PRESET_PREFIX_PATHS = list()
    TARGETS_PREFIX_PATHS = list()

    if TARGETS_BUILD_SYSTEM_NAME == "Windows":
        PRESET_PREFIX_PATHS.append(f"{system_os.path.abspath("build/packages")};") 
        TARGETS_PREFIX_PATHS.append("build/packages;")
    else:
        PRESET_PREFIX_PATHS.append(f"{system_os.path.abspath("build/packages")}:")
        TARGETS_PREFIX_PATHS.append("build/packages:")

    PROJECT_LIBS = [
        ProjectlLibs("glfw", "Vendors/glfw", "build/config/glfw", "build/packages/glfw", ""),
        ProjectlLibs("spdlog", "Vendors/spdlog", "build/config/spdlog", "build/packages/spdlog", ""),
        ProjectlLibs("glad", "Vendors/glad", "build/config/glad", "build/packages/glad", ""),
        ProjectlLibs("glm", "Vendors/glm", "build/config/glm", "build/packages/glm", ""),
        ProjectlLibs("entt", "Vendors/entt", "build/config/entt", "build/packages/entt", ""),
        ProjectlLibs("ImGuiDocking", "Vendors/imgui_docking", "build/config/ImGuiDocking", "build/packages/ImGuiDocking", ""),
        ProjectlLibs("StbImage", "Vendors/stb_image", "build/config/StbImage", "build/packages/StbImage", "")
    ]

    for external_module in PROJECT_LIBS:
        if not system_os.path.exists(external_module.source_directory):
            echo(f"{external_module.name}: MISSING")
            echo("Please run 'git submodule update --init --recursive'")
            sys.exit(1)
        else:
            echo(f"{external_module.name}: FOUND")

#==============================================================================================================================================
    cache_variables = get_build_configurations(TARGETS_BUILD_SYSTEM_NAME, TARGETS_BUILD_TYPE, PROJECT_LIBS)

    if BUILD_TARGETS == "external" or BUILD_TARGETS == "all":
        for external_module in PROJECT_LIBS:
            cmd(f"cmake {cache_variables[1]} {external_module.options} -S {external_module.source_directory} -B {external_module.build_dir}")
            cmd(f"cmake --build {external_module.build_dir} --config {TARGETS_BUILD_TYPE}")
            cmd(f"cmake --install {external_module.build_dir} --config {TARGETS_BUILD_TYPE} --prefix {external_module.prefix_dir}")
 
        generate_preset("Src/Dvicore", cache_variables[0])

#==============================================================================================================================================

    if BUILD_TARGETS == "internal" or BUILD_TARGETS == "all":
        if not system_os.path.exists("Src/Dvicore/CMakePresets.json"):
            echo("CMakePresets.json: MISSING")
            echo("Please build external libs first.")
            sys.exit(1)

        cmd(f"cmake --preset {TARGETS_BUILD_SYSTEM_NAME.lower()}-{TARGETS_BUILD_ARCHITECTURE.lower()}-{TARGETS_BUILD_TYPE.lower()} -S Src/Dvicore -B build/config/Dvicore")
        cmd(f"cmake --build build/config/Dvicore --config {TARGETS_BUILD_TYPE}")
        cmd(f"cmake --install build/config/Dvicore --config {TARGETS_BUILD_TYPE} --prefix build/packages/Dvicore")

        PROJECT_LIBS.append(ProjectlLibs("DviCore", "Src/Dvicore", "build/config/Dvicore", "build/packages/Dvicore", ""))
        cache_variables = get_build_configurations(TARGETS_BUILD_SYSTEM_NAME, TARGETS_BUILD_TYPE, PROJECT_LIBS)

        generate_preset("Src/Dvimana", cache_variables[0])