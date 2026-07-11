cmake_minimum_required(VERSION 4.2)
project(rsmc C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(rsmc SHARED src/rsmc.c
        include/rsmc/types.h)

set_target_properties(rsmc PROPERTIES
        VERSION 0.1.0
        SOVERSION 0
)

target_include_directories(rsmc PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
