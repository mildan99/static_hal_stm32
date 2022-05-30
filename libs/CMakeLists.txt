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
target_include_directories(cmsis-core INTERFACE  ${CMSIS_INCLUDE_DIR})

set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE STRING "")
add_subdirectory(FreeRTOS-Kernel)
target_link_libraries(freertos_kernel cmsis-core)



add_subdirectory(etl)

set(Eigen3_DIR ${CMAKE_CURRENT_SOURCE_DIR}/eigen/cmake/ CACHE STRING "")

