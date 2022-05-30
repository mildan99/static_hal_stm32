message("Toolchain.cmake running.")

# Target operating system
message("Setting : CMAKE_SYSTEM_NAME/VERSION/PROCESSOR")

#
set(CMAKE_SYSTEM_NAME		Generic)
set(CMAKE_SYSTEM_VERSION	1)
set(CMAKE_SYSTEM_PROCESSOR	arm-eabi)


# Name your target information here (Should be the same name as detected by your HAL library #defines)
set(TARGET          STM32F411xE)
set(ARCH			armv7e-m+fp)
set(FPU			    fpv4-sp-d16)
set(CORE			cortex-m4)
set(ARM_ASM			mthumb)



# Compilers to use for ASM, C and C++
message("Setting : Toolchain")
 set( COMPILER_BIN C:/Users/daniil/Desktop/embedded/gcc-arm-none-eabi-11.2/bin )
 set( COMPILER_EXE .exe)
 set(CMAKE_C_COMPILER		${COMPILER_BIN}/arm-none-eabi-gcc${COMPILER_EXE}     CACHE INTERNAL "")
 set(CMAKE_CXX_COMPILER		${COMPILER_BIN}/arm-none-eabi-g++${COMPILER_EXE}     CACHE INTERNAL "")
 set(CMAKE_ASM_COMPILER		${COMPILER_BIN}/arm-none-eabi-g++${COMPILER_EXE}     CACHE INTERNAL "")
 set(CMAKE_OBJCOPY			${COMPILER_BIN}/arm-none-eabi-objcopy${COMPILER_EXE} CACHE INTERNAL "")
set(CMAKE_OBJDUMP			${COMPILER_BIN}/arm-none-eabi-objdump${COMPILER_EXE} CACHE INTERNAL "")
set(CMAKE_SIZE			    ${COMPILER_BIN}/arm-none-eabi-size${COMPILER_EXE}    CACHE INTERNAL "")


# Initialse these options to blank (thats what happens if an unknown build type is selected)
set(MSG_LEN 0)
set(OPTIMISATION "-O0")
set(DEBUG "-g0")
set(ADDITIONAL_FLAGS "")
set(IS_RELEASE TRUE)

# This selects the compiler flags, feel free to change these :)
message("Build mode : " ${CMAKE_BUILD_TYPE})
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Setting up Release flags.")
    set(OPTIMISATION	    "-O2")
    set(ADDITIONAL_FLAGS    "-flto -s  -Wl,--strip-all ")

elseif (CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    message("Setting up MinSizeRel flags.")
    set(OPTIMISATION	    "-Os")
    set(ADDITIONAL_FLAGS    "-flto -s -Wl,--strip-all ")

elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Setting up Debug flags.")
    set(OPTIMISATION	    "-Og ")
    set(DEBUG               "-g3 -ggdb")

elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    message("Setting up RelWithDebInfo flags.")
    set(OPTIMISATION	    "-O2")
    set(DEBUG               "-g3 -ggdb")

elseif (CMAKE_BUILD_TYPE STREQUAL "MinSizeRelWithDebInfo")
    message("Setting up MinSizeRelWithDebInfo flags.")
    set(OPTIMISATION	    "-Os")
    set(DEBUG               "-g3 -ggdb")

else()
    message("WARNING - Unknown build configuration.")
endif()

#
set(COMMON_FLAGS "-mcpu=${CORE} -mfpu=${FPU} -mfloat-abi=hard -mthumb ${OPTIMISATION} ${DEBUG} ${ADDITIONAL_FLAGS} -pedantic -Wall -Wextra -Wfloat-equal -Wshadow  -Wduplicated-branches -Wlogical-op  -fmessage-length=${MSG_LEN} -ffunction-sections -fdata-sections")
set(CPP_FLAGS " -Wnon-virtual-dtor -Woverloaded-virtual -Wsign-promo -Wctor-dtor-privacy -fno-rtti -fno-exceptions -fno-use-cxa-atexit -fno-use-cxa-get-exception-ptr -fno-threadsafe-statics -ftemplate-backtrace-limit=1")

# 
set(CMAKE_ASM_FLAGS	        "${COMMON_FLAGS} -MP -MD -x assembler-with-cpp" CACHE INTERNAL "asm compiler flags")
set(CMAKE_ASM_FLAGS	        "${COMMON_FLAGS}" CACHE INTERNAL "asm compiler flags")
set(CMAKE_C_FLAGS           "${COMMON_FLAGS}" CACHE INTERNAL "c compiler flags")
set(CMAKE_CXX_FLAGS	        "${COMMON_FLAGS} ${CPP_FLAGS}" CACHE INTERNAL "cpp compiler flags")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_RELEASE "")

# You may have a linker issue. If that occurs change nano.specs to nosys.specs
set(CMAKE_EXE_LINKER_FLAGS  "${COMMON_FLAGS} -Wl,-Map,\"build.map\",--wrap=malloc,--wrap=free,--gc-sections --specs=nano.specs -nostartfiles  " CACHE INTERNAL "exe link flags")

# #
# message(${CMAKE_C_FLAGS})
# message(${CMAKE_CXX_FLAGS})
# message(${CMAKE_EXE_LINKER_FLAGS})

# 
message("CMAKE_C_FLAGS = "          ${CMAKE_C_FLAGS})
message("CMAKE_CXX_FLAGS = "        ${CMAKE_CXX_FLAGS})
message("CMAKE_EXE_LINKER_FLAGS = " ${CMAKE_EXE_LINKER_FLAGS})
#
# 
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)