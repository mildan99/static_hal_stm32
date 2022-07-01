cmake_minimum_required(VERSION 3.13)



project(BlackPill-1 VERSION 0.0.1 DESCRIPTION "Test priject")
enable_language(C)
enable_language(CXX)
enable_language(ASM)


if(NOT DEFINED ARTIFACT)
    set(ARTIFACT "build")
endif()

# Add your project #defines here
# These are included as #defines in the project
add_compile_definitions( ${TARGET_MCU}  
                        STM32F411xE
                        HSE_VALUE=25000000)

#
# TODO: Add any additional include directories here
set( PROJECT_INCLUDE_DIRS
    "include"
    "libs/include"
    "libs/Device/ST/STM32F4xx/Include"
    "static_hal/include"
    "scripts"
)

# Setup project sources
file(GLOB_RECURSE 
	 SUPPORT_SOURCE
    "scripts/*.s"
    "static_hal/support/src/*.cpp"
    "static_hal/support/src/*.c"
)

file(GLOB_RECURSE 
    LIBRARY_SOURCE
    "libs/src/*.c"
    "libs/src/*.cpp"
    "static_hal/src/*.c"
    "static_hal/src/*.cpp"
    ${SUPPORT_SOURCE}
)
file(GLOB_RECURSE 
	 MAIN_SOURCE
	"src/*.c"
    "src/*.cpp"
    ${SUPPORT_SOURCE}
)



# Add all linker scripts to the linker command
file(GLOB_RECURSE
	LK_SCRIPTS
	"scripts/*.ld"
)

set(LINKER_COMMON "")
foreach(file ${LK_SCRIPTS})
	set(LINKER_COMMON "${LINKER_COMMON} -T \"${file}\"")
endforeach()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}${LINKER_COMMON}" CACHE INTERNAL "exe link flags")


# set arch for freertos
set(FREERTOS_PORT GCC_ARM_CM4F CACHE STRING "")
add_subdirectory(libs)



 
add_library(static_hal STATIC ${LIBRARY_SOURCE})
target_include_directories(static_hal PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries(static_hal PUBLIC ${LINKER_LIBS} libs::cmsis-core libs::freertos etl::etl libs::eigen libs::sml libs::ctre)
target_compile_features(static_hal PUBLIC cxx_std_20)
target_compile_options(static_hal PUBLIC -Wno-volatile)
target_compile_definitions(static_hal PUBLIC EIGEN_NO_MALLOC EIGEN_NO_IO EIGEN_DONT_VECTORIZE)

add_executable(${ARTIFACT} ${MAIN_SOURCE} )
target_link_libraries(${ARTIFACT} PUBLIC static_hal)
#
# Print the size of the .hex
add_custom_target(${ARTIFACT}.bin ALL DEPENDS ${ARTIFACT} COMMAND ${CMAKE_OBJCOPY} -Obinary ${ARTIFACT}.elf ${ARTIFACT}.bin)
add_custom_target(map_demanlge ALL DEPENDS ${ARTIFACT} COMMAND powershell.exe -Command \"cat output.map |arm-none-eabi-c++filt.exe > demap.map\" )
# powershell.exe -Command "cat .\build\output.map |arm-none-eabi-c++filt.exe >build/demap.map"
