cmake_minimum_required(VERSION 3.16.0)
project(deps )

enable_language(C)




if((NOT CMSIS_INCLUDE_DIR) OR (NOT EXISTS ${FOO_INCLUDE_DIR}))
    # we couldn't find the header files for FOO or they don't exist
 
    # we have a submodule setup for foo, assume it is under external/foo
    # now we need to clone this submodule
    execute_process(COMMAND git submodule update --init -- ./CMSIS_5
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


else()
    # see above, setup target as well
endif()



# set INCLUDE_DIR properly
set(CMSIS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_5/CMSIS/Core/Include
    CACHE PATH "CMSIS include directory")
message("Cmsis include: ${CMSIS_INCLUDE_DIR}")



add_library(cmsis-core INTERFACE)
target_include_directories(cmsis-core INTERFACE  ${CMSIS_INCLUDE_DIR})

set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE STRING "")
add_subdirectory(FreeRTOS-Kernel)
target_link_libraries(freertos_kernel cmsis-core)


add_subdirectory(etl)
