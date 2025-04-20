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

add_executable(engine ${SOURCES})

if(WIN32)
    file(GLOB SDL2_LIB "${CMAKE_SOURCE_DIR}/lib/*.lib")
    file(GLOB LUAJIT_LIB "${CMAKE_SOURCE_DIR}/lib/*.lib")

    target_link_libraries(engine
        ${SDL2_LIB}
        ${CMAKE_SOURCE_DIR}/lib/luajit.lib
    )
elseif(UNIX)
    include_directories(${SDL2_INCLUDE_DIRS} ${LUAJIT_INCLUDE_DIRS})
    link_directories(${SDL2_LIBRARY_DIRS} ${LUAJIT_LIBRARY_DIRS})

    target_link_libraries(engine
        ${SDL2_LIBRARIES}
        ${LUAJIT_LIBRARIES}
        luajit
        dl  # just in case LuaJIT needs dynamic linking
        m   # math lib, commonly needed
    )
endif()

