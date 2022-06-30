cmake_minimum_required(VERSION 3.16.0)
project(deps )

enable_language(C)




if((NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_5))
    execute_process(COMMAND git submodule update --init -- ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_5
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

endif()

if((NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel))
    execute_process(COMMAND git submodule update --init -- ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

endif()

if((NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/etl))
    execute_process(COMMAND git submodule update --init -- ${CMAKE_CURRENT_SOURCE_DIR}/etl
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

endif()
if((NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/eigen))
    execute_process(COMMAND git submodule update --init -- ${CMAKE_CURRENT_SOURCE_DIR}/eigen
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()
# set INCLUDE_DIR properly
set(CMSIS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_5/CMSIS/Core/Include
    CACHE PATH "CMSIS include directory")



add_library(cmsis-core INTERFACE)
target_include_directories(cmsis-core SYSTEM INTERFACE  ${CMSIS_INCLUDE_DIR})

set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE STRING " ")
add_subdirectory(FreeRTOS-Kernel)
target_link_libraries(freertos_kernel cmsis-core)

add_library(eigen INTERFACE)
target_include_directories(eigen  SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/eigen)


add_subdirectory(etl)

if((EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/sml))
    add_subdirectory(sml)
    add_library(libs::sml ALIAS sml)

endif()
if((EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ctre))
    add_subdirectory(ctre)
    add_library(libs::ctre ALIAS ctre)
endif()

add_library(libs::eigen ALIAS eigen)
add_library(libs::freertos ALIAS freertos_kernel)
add_library(libs::cmsis-core ALIAS cmsis-core)

