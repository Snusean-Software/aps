cmake_minimum_required( VERSION 3.0 )
project( os )

set( CMAKE_CXX_STANDARD 17 )
set( PROJECT_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/bin )

set( SOURCES
        src/global.cpp
        src/os.cpp
        src/resource.cpp
        src/task.cpp
        src/events.cpp
        test.cpp
)

add_executable( test ${SOURCES} )

target_include_directories( test PUBLIC
     include
     src
)

if( APPLE )
        target_compile_definitions( ${PROJECT_NAME} PRIVATE _XOPEN_SOURCE=600 )
endif()
