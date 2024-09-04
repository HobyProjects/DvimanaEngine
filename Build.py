#!/usr/bin/env python

import os
import sys
import argparse

# Create an argument parser to process command line arguments
# ----------------------------------------------------------
# The argument parser is an object that parses the command line
# arguments passed to the script.  It is used to add arguments to
# the script and to parse the arguments passed to the script.
# ----------------------------------------------------------
parser = argparse.ArgumentParser(
    description="Build script for the Dvimana Engine"
)


# Add a command line argument to select the build configuration
# ----------------------------------------------------------
# The --config argument is added to the parser with the following options:
# - type: str, The argument is a string
# - default: "Debug", The default value if the argument is not provided
# - help: A description of the argument, which is displayed when the help option is used
parser.add_argument(
    "--config",
    type=str,
    help="The build configuration to use. The following values are valid:\n"
         "  - Debug: Build with debugging symbols and no optimizations.\n"
         "  - Release: Build with optimizations but no debugging symbols.\n"
)

# Parse the command line arguments
# --------------------------------
# The parse_args() method is called to parse the command line arguments
# The result is stored in the args variable
args = parser.parse_args()

def cmd(command):
    """
    Execute a shell command

    Parameters
    ----------
    command : str
        The command to execute

    Returns
    -------
    None

    Raises
    ------
    Exception
        If the command fails to execute

    Examples
    --------
    >>> cmd("ls")
    """
    try:
        print("Executing command: " + command)
        os.system(command)
    except Exception as e:
        print(e)
        print("Failed to execute command: " + command)
        sys.exit(1)


# Constants
# ---------
BUILD_PATH = "build"                # The path to the build directory
CONFIG_PATH = "build/config"        # The path to the config directory
PREFIX_PATH = "build/packages"      # The path to the packages directory
SUBMODULES_PATH = "Vendors"         # The path to the submodules directory
SUBMODULES = [ "glfw", "spdlog", "glew-cmake", "imgui_docking" ]

def verify_submodules():
    """
    Verify that all the submodules are present

    Parameters
    ----------
    None

    Returns
    -------
    None

    """
    for submodule in SUBMODULES:
        if not os.path.isdir(f"{SUBMODULES_PATH}/{submodule}"):
            print(f"Missing submodule: {submodule}")
            print("Please run 'git submodule update --init --recursive'")
            sys.exit(1)


# Verify that all the submodules are present
# ------------------------------------------------
# This function is called once at the beginning of the script
# to verify that all the submodules are present.  If any are missing
# the script will exit with an error message.
# ------------------------------------------------
verify_submodules()

# Check that the build configuration is valid
# -------------------------------------------
if args.config != "Debug" and args.config != "Release":
    print("Invalid build configuration: " + args.config)
    print("Valid configurations are: Debug, Release")
    sys.exit(1)

# Set the BUILD_TYPE variable if the build configuration was Release
# -----------------------------------------------------------------
if args.config == "Release":
    BUILD_TYPE = "Release"
else:
    BUILD_TYPE = "Debug"

print("Building with configuration: " + BUILD_TYPE)


# Set the CMAKE_CACHE_VARIABLES string with the build type and the prefix path
# ----------------------------------------------------------------------
# The CMAKE_CACHE_VARIABLES string is used to pass variables to CMake that are
# not in the cache. This string is used as a command line argument to CMake.
#
# The -DCMAKE_BUILD_TYPE= argument is used to specify the build type. The value
# of this argument is the build type that was specified on the command line.
#
# The -DCMAKE_INSTALL_PREFIX= argument is used to specify the directory where
# the files will be installed to. The value of this argument is the PREFIX_PATH
# variable, which is set to the "packages" directory in the build directory.
#
# The -DCMAKE_PREFIX_PATH= argument is used to specify the directory where the
# dependencies will be installed to. The value of this argument is the PREFIX_PATH
# variable, which is set to the "packages" directory in the build directory.
CMAKE_CACHE_VARIABLES = f"""
-DCMAKE_BUILD_TYPE={BUILD_TYPE} 
-DCMAKE_INSTALL_PREFIX={PREFIX_PATH} 
-DCMAKE_PREFIX_PATH={PREFIX_PATH}
""".replace("\n", " ").replace("\t", " ")

# Loop through each submodule and do the following:
#  1. Generate the build files for the submodule
#  2. Build the submodule
#  3. Install the submodule
for submodule in SUBMODULES:
    # Step 1: Generate the build files for the submodule
    # ---------------------------------------------------
    # The `-S` option specifies the source directory of the submodule
    # The `-B` option specifies the directory where the build files will be placed
    # The `-DCMAKE_BUILD_TYPE` option specifies the build type
    # The `-DCMAKE_INSTALL_PREFIX` option specifies the directory where the submodule will be installed
    # The `-DCMAKE_PREFIX_PATH` option specifies the directory where the submodule and its dependencies will be installed
    cmd(f"cmake {CMAKE_CACHE_VARIABLES} -S {SUBMODULES_PATH}/{submodule} -B {CONFIG_PATH}/{submodule}")

    # Step 2: Build the submodule
    # --------------------------
    # The `--build` option tells CMake to build the submodule
    # The `{CONFIG_PATH}/{submodule}` argument specifies the directory where the build files are located
    # The `--config {BUILD_TYPE}` option specifies the build type
    cmd(f"cmake --build {CONFIG_PATH}/{submodule} --config {BUILD_TYPE}")

    # Step 3: Install the submodule
    # ---------------------------
    # The `--install` option tells CMake to install the submodule
    # The `{CONFIG_PATH}/{submodule}` argument specifies the directory where the build files are located
    # The `--config {BUILD_TYPE}` option specifies the build type
    # The `--prefix {PREFIX_PATH}` option specifies the directory where the submodule will be installed
    cmd(f"cmake --install {CONFIG_PATH}/{submodule} --config {BUILD_TYPE} --prefix {PREFIX_PATH}")



