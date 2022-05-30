This is a template for cmake projects targetting embedded systems.

To get started follow these steps:

1. Your chip vendor will likely provide a .svd file for your microcontroller, place it in the micro folder and rename it micro.svd.

2. If your chip vendor provides an SDK place it into the micro/target directory.

3. Add the source folders to the CMakeLists.txt file( line 48 and 49 ), you'll need to override the contents of the stm32 HAL library includes.

4. Add the header folders to the CMakeLists.txt file( line 37 to and including 41 ), you'll need to override the includes in that range with your platform sdk includes.

5. In the CMakeLists.txt file you'll need to setup the TARGET, ARCH, CORE and ASM_ASM definitions, these are found at line 4 to line 7.

6. At line 20 of CMakeLists.txt configure your project name (override ProjectTemplate). You can also configure your project version and description here.

7. Configure whether the project is a STATIC_LIBRARY or EXECUTABLE, this is done at line 26 of the CMakeLists.txt file.

8. Your SDK may require a series of defines for compliation to complete, override the ones provided at line 31 of CMakeLists.txt with your required ones.

9. In vs code settings search for openocd, you'll need to configure the paths to openocd, ninja and cmake. These are done in the global vscode settings (that way it'll work with other projects, you don't have to do it that way if you don't want)

You'll need atleast the following included in the settings.json:

    "cmake.configureOnOpen": true,
    "cmake.installPrefix": "",
    "cmake.cmakePath": "##PATH TO YOUR CMAKE EXE##",
    "cmake.generator": "Ninja",
    "cmake.configureSettings": {
        "CMAKE_MAKE_PROGRAM" : "##PATH TO THE NINJA EXE##"
    },
    "cortex-debug.openocdPath": "##PATH TO THE OPENOCD EXE##",

To use it you need the following tools:
- vscode https://code.visualstudio.com/
- gnu gcc arm embedded toolchain https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm
- cmake https://cmake.org/download/
- ninja build system https://ninja-build.org/

If you want debugging you need to install:
- openocd https://gnutoolchains.com/arm-eabi/openocd/

In vscode you need the following plugins:
- CMake by twxs
- CMake Tools by vector-of-bool

If you need debugging you need the plugin:
- Cortex-Debug by marus25