#!/usr/bin/env python

import os as system_os
import sys
import json
import argparse

def cmd(command):
    """
    Run a command in the shell.

    If the command fails to run for any reason, this function will print an
    error message and exit the program with a non-zero status code.

    Parameters
    ----------
    command : str
        The command to run in the shell.

    Returns
    -------
    None
    """
    try:
        # Print a message before running the command so that we know what's
        # going on.
        print("Executing command: " + command)

        # Run the command in the shell. This will block until the command
        # finishes.
        system_os.system(command)
    except Exception as e:
        # If anything goes wrong while running the command, catch the
        # exception and print an error message.
        print(e)

        # Print a message to remind the user what command caused the error.
        print("Failed to execute command: " + command)

        # Exit the program with a non-zero status code. This will prevent
        # further commands from running.
        sys.exit(1)

class ProjectExternalLibs:
    """
    This class represents a project that is not part of the main Dvimana
    project. This can be a library that we depend on, or a tool that we
    use to build the project.

    Attributes
    ----------
    name : str
        The name of the project. This is used to identify the project
        in the build system.
    src_dir : str
        The directory where the source code for the project is stored.
        This is used by the build system to find the source code.
    build_dir : str
        The directory where the build artifacts for the project are
        stored. This is used by the build system to store the build
        artifacts.
    prefix_dir : str
        The directory where the build artifacts for the project are
        installed. This is used by the build system to install the
        build artifacts.
    """

    def __init__(self, name, dir):
        """
        Initialize a ProjectExternalLibs object.

        Parameters
        ----------
        name : str
            The name of the project.
        dir : str
            The directory where the source code for the project is
            stored.
        """
        self.name = name
        self.src_dir = f"Vendors/{dir}"
        self.build_dir = f"build/config/{name}"
        self.prefix_dir = f"build/packages/{name}"

    def __str__(self):
        """
        Return a string representation of the ProjectExternalLibs object.

        Returns
        -------
        str
            A string representation of the ProjectExternalLibs object.
        """
        return f"Name: {self.name}\nDir: {self.dir}\nBuild Dir: {self.build_dir}\nPrefix: {self.prefix}\n"

#====================================[ CMakePresets Generator Function ]============================================================================================================   

def get_base_configure_preset():
    """
    Returns a dictionary representing a base configure preset for all platforms.

    The 'name' field is set to 'common-base' to indicate that this preset is a base preset that
    all other presets will inherit from. The 'hidden' field is set to True, meaning that this
    preset won't be shown in the CMake GUI. The 'binaryDir' and 'installDir' fields are set to
    directories that are relative to the source directory ('${sourceDir}').

    The '${presetName}' variable is a special variable that is replaced by the name of the
    preset when a preset is created. This allows us to create a unique directory for each
    preset.
    """

    return {
        "name": "common-base",
        "hidden": True,
        "binaryDir": "${sourceDir}/build/${presetName}",
        "installDir": "${sourceDir}/out/${presetName}"
    }

def get_os_base_configure_preset(os: str, inherits: str, cache_variables: list):
    """
    Returns a dictionary representing a base configure preset for a specific OS.

    The 'name' field is set to the lowercase name of the OS plus '-base',
    indicating that this preset is a base preset that all other presets for
    this OS will inherit from. The 'hidden' field is set to True, meaning that
    this preset won't be shown in the CMake GUI.

    The 'condition' field is set to a dictionary that specifies a condition
    under which this preset is valid. The condition is that the 'hostSystemName'
    variable must be equal to the given OS name. This allows us to create a
    different preset for each OS.

    The 'cacheVariables' field is set to a dictionary that specifies the
    cache variables that should be set for this preset. The values are taken
    from the given list.

    The 'vendor' field is set to a dictionary that specifies the vendor-specific
    settings for this preset. In this case, the vendor is 'microsoft.com' and
    the settings are for Visual Studio. The 'hostOS' setting is set to the given
    OS name, and the 'sourceDir' setting is set to the directory that contains
    the '.vs' directory, which is where the CMakeCache.txt file is located.

    Returns
    -------
    dict
        A dictionary representing a base configure preset for a specific OS.
    """
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
    """
    Returns a dictionary representing a CMake preset for a specific OS.

    The 'name' field is set to a string that uniquely identifies the preset.
    The string is constructed by concatenating the lowercased OS name, the
    architecture string, and the lowercased configuration string.

    The 'inherits' field is set to the name of the base preset that this preset
    inherits from. This allows us to create a hierarchy of presets.

    The 'displayName' field is set to a string that is displayed to the user
    when they select this preset. The string is constructed by concatenating the
    architecture string and the configuration string.

    The 'architecture' field is set to a dictionary that specifies the
    architecture for this preset. The 'value' field is set to the architecture
    string, and the 'strategy' field is set to 'external', which means that the
    architecture is determined by the CMake generator.

    The 'cacheVariables' field is set to a dictionary that specifies the cache
    variables that should be set for this preset. The only cache variable that
    is set is 'CMAKE_BUILD_TYPE_INIT', which is set to the configuration string
    (i.e. 'Debug' or 'Release').

    Returns
    -------
    dict
        A dictionary representing a CMake preset for a specific OS.
    """

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

#======================================================================================================================================================

if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        description="Build script for the Dvimana Engine"
    )

    # Add a command line argument to specify the build configuration.
    # The user can specify one of two valid values: 'Debug' or 'Release'.
    # If 'Debug' is specified, the build will be done with debugging symbols but
    # without optimizations. If 'Release' is specified, the build will be done with
    # optimizations but without debugging symbols.
    parser.add_argument(
        "--config",
        type=str,
        help="The build configuration to use. The following values are valid:"
            "  - Debug: Build with debugging symbols and no optimizations."
            "  - Release: Build with optimizations but no debugging symbols."
    )


    # Add a command line argument to specify the target architecture.
    # The user can specify one of two valid values: 'x86' or 'x64'.
    # If 'x86' is specified, the build will be done for x86 architecture.
    # If 'x64' is specified, the build will be done for x64 architecture.
    parser.add_argument(
        "--arch",
        type=str,
        help="The architecture to build for. The following values are valid:"
            "  - x86: Build for x86 architecture."
            "  - x64: Build for x64 architecture."
    )

    # Add a command line argument to specify the target to build for.
    # The user can specify one of three valid values: 'external', 'internal', or 'all'.
    # If 'external' is specified, the build will be done for the external libraries and executables only.
    # If 'internal' is specified, the build will be done for the internal libraries and executables only.
    # If 'all' is specified, the build will be done for all libraries and executables.
    parser.add_argument(
        "--target",
        type=str,
        help="The target to build for. The following values are valid:"
            " - external: Build external libraries / executables only"
            " - internal: Build internal libraries / executables only"
            " - all: Build all libraries / executables"
    )

    args = parser.parse_args()

    # Check that the build configuration argument is valid.
    # If it's not "Debug" or "Release", print an error message
    # and exit the program.
    if args.config != "Debug" and args.config != "Release":
        print(f"-- Invalid build configuration: {args.config}")
        print("-- Valid configurations are: Debug, Release")
        print("")
        print("The build configuration determines whether the build is done with")
        print("debugging symbols and no optimizations, or with optimizations but")
        print("no debugging symbols.")
        sys.exit(1)

    # Check that the target architecture argument is valid.
    # If it's not "x86" or "x64", print an error message and exit the program.
    if args.arch != "x86" and args.arch != "x64":
        print(f"-- Invalid architecture: {args.arch}")
        print("-- Valid architectures are: x86, x64")
        print("")
        print("The target architecture determines whether the build is done for")
        print("x86 or x64 architecture.")
        sys.exit(1)

    # Check that the target argument is valid.
    # If it's not "external", "internal", or "all", print an error message and exit the program.
    if args.target != "external" and args.target != "internal" and args.target != "all":
        print(f"-- Invalid target: {args.target}")
        print("-- Valid targets are: external, internal, all")
        print("")
        print("The target determines what libraries and executables will be built.")
        print("If 'external' is specified, the build will be done for the external")
        print("libraries and executables only. If 'internal' is specified, the build")
        print("will be done for the internal libraries and executables only. If 'all'")
        print("is specified, the build will be done for all libraries and executables.")
        sys.exit(1)

    # Store the build configuration, target architecture, and target in variables
    # so they can be used later in the script.
    TARGETS_BUILD_TYPE = args.config
    TARGETS_BUILD_ARCHITECTURE = args.arch
    BUILD_TARGETS = args.target
    TARGETS_BUILD_SYSTEM_NAME = str()

    # Determine the build system name based on the system platform.
    # If the platform is Windows, set the build system name to "Windows", if it's
    # MacOS, set it to "MacOS", and if it's anything else, set it to "Linux".

    if sys.platform == "win32":
        TARGETS_BUILD_SYSTEM_NAME = "Windows"

    elif sys.platform == "darwin":
        TARGETS_BUILD_SYSTEM_NAME = "MacOS"

    else:
        TARGETS_BUILD_SYSTEM_NAME = "Linux"

#======================================================================================================================================================

    # These lists will be used to store the paths to the external libraries and
    # executables that we will build. The paths will be stored in a format that
    # can be used with the CMake command line option -DCMAKE_PREFIX_PATH.
    PRESET_PREFIX_PATHS = list()
    TARGETS_PREFIX_PATHS = list()

    # The CMAKE_PREFIX_PATH is a semicolon-separated list of paths to search
    # for external libraries and executables. The paths in the list should be
    # absolute paths to the prefix directories of the libraries and executables.
    # The prefix directory is the directory that contains the include, lib,
    # and bin directories of the library or executable.
    if TARGETS_BUILD_SYSTEM_NAME == "Windows":
        # On Windows, the paths in the list are separated by semicolons.
        PRESET_PREFIX_PATHS.append(f"{system_os.path.abspath("build/packages")};") 
        TARGETS_PREFIX_PATHS.append("build/packages;")
    else:
        # On other platforms, the paths in the list are separated by colons.
        PRESET_PREFIX_PATHS.append(f"{system_os.path.abspath("build/packages")}:")
        TARGETS_PREFIX_PATHS.append("build/packages:")


    # These are the external libraries that we will build.
    PROJECT_EXTERNALS = [
        ProjectExternalLibs("glfw", "glfw"),
        ProjectExternalLibs("spdlog", "spdlog"),
        ProjectExternalLibs("glew", "glew-cmake"),
        ProjectExternalLibs("glm", "glm"),
        ProjectExternalLibs("entt", "entt"),
        ProjectExternalLibs("ImGuiDocking", "imgui_docking"),
        ProjectExternalLibs("StbImage", "stb_image")
    ]

    # Iterate over the external libraries and add the path to their prefix
    # directories to the list of paths.
    for external_module in PROJECT_EXTERNALS:
        if not system_os.path.exists(external_module.src_dir):
            print(f"-- {external_module.name}: MISSING")
            print("Please run 'git submodule update --init --recursive'")
            sys.exit(1)
        else:
            print(f"-- {external_module.name}: FOUND")
            if TARGETS_BUILD_SYSTEM_NAME == "Windows":
                # On Windows, the paths in the list are separated by semicolons.
                PRESET_PREFIX_PATHS.append(f"{system_os.path.abspath(external_module.prefix_dir)};")
                TARGETS_PREFIX_PATHS.append(f"{external_module.prefix_dir};")
            else:
                # On other platforms, the paths in the list are separated by colons.
                PRESET_PREFIX_PATHS.append(f"{system_os.path.abspath(external_module.prefix_dir)}:")
                TARGETS_PREFIX_PATHS.append(f"{external_module.prefix_dir}:")


    # Convert the list of paths to a string that can be used with the CMake
    # command line option -DCMAKE_PREFIX_PATH.
    CMAKE_PREFIX_PATHS_STRING = "".join(TARGETS_PREFIX_PATHS)
    PRESET_PREFIX_PATHS_STRING = "".join(PRESET_PREFIX_PATHS)

    # Create the string that will be used as the CMake command line cache
    # variable. This variable will be used to initialize the CMake cache
    # when generating the build files.
    CMAKE_CACHE_VARIABLES = f"-DCMAKE_BUILD_TYPE_INIT={TARGETS_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX={CMAKE_PREFIX_PATHS_STRING} -DCMAKE_PREFIX_PATH={CMAKE_PREFIX_PATHS_STRING}"
    PRESET_CACHE_VARIABLES = [ TARGETS_BUILD_TYPE, PRESET_PREFIX_PATHS_STRING, PRESET_PREFIX_PATHS_STRING ]

    # If the target is "external" or "all", then build the external libraries.
    if BUILD_TARGETS == "external" or BUILD_TARGETS == "all":
        # Iterate over the external libraries and build them.
        for external_module in PROJECT_EXTERNALS:
            # Generate the build files for the library.
            cmd(f"cmake {CMAKE_CACHE_VARIABLES} -S {external_module.src_dir} -B {external_module.build_dir}")
            # Build the library.
            cmd(f"cmake --build {external_module.build_dir} --config {TARGETS_BUILD_TYPE}")
            # Install the library.
            cmd(f"cmake --install {external_module.build_dir} --config {TARGETS_BUILD_TYPE} --prefix {external_module.prefix_dir}")

# =====================================================[ Writing CMake Presets ]=========================================================================================

    # Initialize lists to hold the presets for configuring, building, and testing
    # the project. These lists will be used to generate the CMakePresets.json file.
    configure_presets = []
    build_presets = []
    test_presets = []

    # Define the base configure preset, which is a common preset for all
    # platforms. This preset includes the basic settings that are used by all
    # platforms.
    base_configure_preset = get_base_configure_preset()

    # Define the base build preset, which is a common preset for all platforms.
    # This preset includes the basic settings that are used by all platforms.
    base_build_preset = {
        'name': 'common-base',
        'hidden': True,
        'jobs': 1,
        'cleanFirst': False
    }

    # Define the base test preset, which is a common preset for all platforms.
    # This preset includes the basic settings that are used by all platforms.
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

    # Add the base configure preset to the list of configure presets.
    configure_presets.append(base_configure_preset)

    # Add the base build preset to the list of build presets.
    build_presets.append(base_build_preset)

    # Add the base test preset to the list of test presets.
    test_presets.append(base_test_preset)

    # Determine if the build type is debug or release.
    build_type_debug = True if TARGETS_BUILD_TYPE == "Debug" else False
    build_type_release = True if TARGETS_BUILD_TYPE == "Release" else False

    # Define a list of operating systems that we want to support.
    os_names = ["Linux" , "Windows", "macOS"]

    # Define a list of build configurations that we want to support.
    configs = [ "Debug", "Release"]

    # Iterate over each operating system and build configuration.
    for _os in os_names:
        # Get the base configure preset for the current operating system.
        os_base_configure_preset = get_os_base_configure_preset(_os, base_configure_preset["name"], PRESET_CACHE_VARIABLES)

        # Add the base configure preset for the current operating system to the list
        # of configure presets.
        configure_presets.append(os_base_configure_preset)

        # Iterate over each build configuration.
        for conf in configs:
            # Get the configure preset for the current operating system and build
            # configuration.
            conf_preset = get_os_preset(_os, os_base_configure_preset["name"], "x64", conf)

            # Add the configure preset for the current operating system and build
            # configuration to the list of configure presets.
            configure_presets.append(conf_preset)

            # Get the configure preset for the current operating system and build
            # configuration, but for the x86 architecture.
            conf_preset = get_os_preset(_os, os_base_configure_preset["name"], "x86", conf)

            # Add the configure preset for the current operating system and build
            # configuration, but for the x86 architecture, to the list of
            # configure presets.
            configure_presets.append(conf_preset)

        # Add the build preset for the current operating system and build
        # configuration to the list of build presets.
        build_presets.append({"name": os_base_configure_preset["name"], "inherits": base_build_preset["name"], "hidden": True})

        # Add the test preset for the current operating system and build
        # configuration to the list of test presets.
        test_presets.append({"name": os_base_configure_preset["name"], "inherits": base_test_preset["name"], "hidden": True})

    # Iterate over the configure presets and add a build preset and a test preset
    # for each one.
    for conf_preset in configure_presets:
        if conf_preset.get('hidden', False) == True:
            continue

        # Add the build preset for the current configure preset to the list of
        # build presets.
        build_presets.append({
            "name": conf_preset["name"],
            "inherits": conf_preset["inherits"],
            "displayName": conf_preset["displayName"],
            "configurePreset": conf_preset["name"],
        })

        # Add the test preset for the current configure preset to the list of
        # test presets.
        test_presets.append({
            "name": conf_preset["name"],
            "inherits": conf_preset["inherits"],
            "displayName": conf_preset["displayName"],
            "configurePreset": conf_preset["name"],
        })

    # Create the root presets object, which will be used to write the
    # CMakePresets.json file.
    root_presets = {
        "version": 3,
        "configurePresets": configure_presets,
        "buildPresets": build_presets,
        "testPresets": test_presets
    }

    # If the CMakePresets.json file already exists, then delete it.
    if system_os.path.exists("Src/CMakePresets.json"):
        system_os.remove("Src/CMakePresets.json")

    # Open the CMakePresets.json file and write the root presets object to it.
    with open("Src/CMakePresets.json", 'a+') as f:
        json.dump(root_presets, f, indent=2)

#==============================================================================================================================================
    
    if BUILD_TARGETS == "all" or BUILD_TARGETS == "internal":
        cmd(f"cmake --preset {TARGETS_BUILD_SYSTEM_NAME.lower()}-{TARGETS_BUILD_ARCHITECTURE.lower()}-{TARGETS_BUILD_TYPE.lower()} -S Src -B Src/build/config")
        cmd(f"cmake --build Src/build/config --config {TARGETS_BUILD_TYPE}")