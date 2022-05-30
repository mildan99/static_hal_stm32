cmake_minimum_required(VERSION 3.13)



# TODO: You may need to configure this.
include(scripts/CMake/Toolchain.cmake)

# TODO: Replace ProjectTemplate with your project name.
# TODO: Replace 1.0.0 with your project version.
# TODO: Replace "Description" with your project description.
# Name your project here
project(BlackPill-1 VERSION 0.0.1 DESCRIPTION "Test priject")
enable_language(C)
enable_language(CXX)
enable_language(ASM)
# Configure the C and C++ standards
SET(CMAKE_C_STANDARD 11)
SET(CMAKE_CXX_STANDARD 17)
#
# Select from:
# - STATIC_LIBRARY
# - EXECUTABLE
set(TYPE EXECUTABLE)

# Project mode, precompiled .lib file or .elf file?

# Add your project #defines here
# These are included as #defines in the project
add_compile_definitions( ${TARGET}  STM32F411xE
                                    ARM_MATH_CM4
                                    HSE_VALUE=25000000
                                    EIGEN_DONT_VECTORIZE
                                    EIGEN_NO_IO
                                    EIGEN_NO_MALLOC  )

#
# TODO: Add any additional include directories here
include_directories(
    "include"
    "libs/include"
    "libs/Device/ST/STM32F4xx/Include"
    "libs/superlib/include"
    "scripts"

)

# TODO: Add your project source files or their containing folders here
# Setup project sources
file(GLOB_RECURSE 
    LIBRARY_SOURCE
    "libs/src/*.c"
    "libs/src/*.cpp"
    "libs/superlib/src/*.c"
    "libs/superlib/src/*.cpp"
)
file(GLOB_RECURSE 
	 MAIN_SOURCE
     ${LIBRARY_SOURCE}
	"src/*.c"
    "src/*.cpp"
    "scripts/*.s"
)

# set(LINKER_LIBS "${CMAKE_SOURCE_DIR}/libs/CMSIS/Lib/GCC/libarm_cortexM4lf_math.a")
set(LINKER_LIBS "")

# TODO: Add your project linker scripts here (if you have any)
# Add all linker scripts to the linker command
file(GLOB_RECURSE
	LK_SCRIPTS
	"scripts/*.ld"
)
#

# This only does something if .ld files exist in the scripts directory
set(LINKER_COMMON "")
#
#
foreach(file ${LK_SCRIPTS})
	set(LINKER_COMMON "${LINKER_COMMON} -T \"${file}\"")
endforeach()
#
#
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}${LINKER_COMMON}" CACHE INTERNAL "exe link flags")
##
##
message("CMAKE_EXE_LINKER_FLAGS = " ${CMAKE_EXE_LINKER_FLAGS})
#
#




set(FREERTOS_PORT GCC_ARM_CM4F CACHE STRING "")
add_subdirectory(libs)


if (TYPE STREQUAL "STATIC_LIBRARY")
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
    #
    # TODO: If the project should be build as a static library uncomment this
    add_library( build STATIC ${MAIN_SOURCE} )
    target_link_libraries(build ${LINKER_LIBS})
else()
    # 
    add_executable(build.elf ${MAIN_SOURCE})
    target_link_libraries(build.elf PUBLIC ${LINKER_LIBS} cmsis-core freertos_kernel etl::etl Eigen3::Eigen)
  #  configure_file(build/compile_commands.json ../compile_commands.json COPYONLY)

    #message(${CMAKE_SOURCE_DIR})
    #
    # Print the size of the .hex
    add_custom_target(size ALL ${CMAKE_SIZE} build.elf DEPENDS build.elf)
    add_custom_target(build.bin ALL DEPENDS build.elf COMMAND ${CMAKE_OBJCOPY} -Obinary build.elf build.bin)
endif()