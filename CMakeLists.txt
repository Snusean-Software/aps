cmake_minimum_required(VERSION 3.19)
project(SAP_OS)

set(CMAKE_CXX_STANDARD 17)
set( PROJECT_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/bin )

add_executable(SAP_OS
        defs.h
        global.cpp
        os.cpp
        resource.cpp
        rtos_api.h
        sys.h
        task.cpp
        test.cpp events.cpp)

if(APPLE)
        target_compile_definitions(SAP_OS PRIVATE _XOPEN_SOURCE=600)
endif()
