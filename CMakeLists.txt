cmake_minimum_required( VERSION 3.19 )
project( os )

set( CMAKE_CXX_STANDARD 17 )
set( PROJECT_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/bin )

set( SOURCES
        global.cpp
        os.cpp
        resource.cpp
        task.cpp
        test.cpp
        events.cpp
)

add_executable( ${PROJECT_NAME} ${SOURCES} )

if( APPLE )
        target_compile_definitions( ${PROJECT_NAME} PRIVATE _XOPEN_SOURCE=600 )
endif()
