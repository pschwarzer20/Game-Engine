cmake_minimum_required(VERSION 3.12)

project(engine)

# C++ standard
set(CMAKE_CXX_STANDARD 20)

# Source files
set(SOURCES
    main.cpp
    main.h
    ${CMAKE_SOURCE_DIR}/include/glad.c
)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

file(GLOB SDL2_LIB "${CMAKE_SOURCE_DIR}/lib/*.lib")
file(GLOB LUAJIT_LIB "${CMAKE_SOURCE_DIR}/lib/*.lib")

add_executable(engine ${SOURCES})

if(WIN32)
elseif(UNIX)
endif()

# Link all required libraries
target_link_libraries(engine
    ${SDL2_LIB}
    ${CMAKE_SOURCE_DIR}/lib/luajit.lib
)
