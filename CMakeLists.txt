cmake_minimum_required(VERSION 3.28)
project(rsmc C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(rsmc SHARED
        src/rsmc.c
        include/rsmc.h
        include/rsmc/types.h
)

target_include_directories(rsmc PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(rsmc PROPERTIES
        VERSION 0.1.0
        SOVERSION 0
        WINDOWS_EXPORT_ALL_SYMBOLS ON
)

include(GNUInstallDirs)

install(TARGETS rsmc
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

option(RSMC_BUILD_TESTS "Build rsmc test suite" ${PROJECT_IS_TOP_LEVEL})

if(RSMC_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(unity
            URL https://github.com/ThrowTheSwitch/Unity/archive/refs/tags/v2.6.1.zip
            URL_HASH SHA256=a27c9779a6ca56a0c5d9b59e139d1c7992eb85254a00add52eaf74a5254aa9f0
            EXCLUDE_FROM_ALL
            SYSTEM
    )
    FetchContent_MakeAvailable(unity)

    enable_testing()
    add_subdirectory(test)
endif()
