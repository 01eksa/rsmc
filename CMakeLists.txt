cmake_minimum_required(VERSION 3.28)
project(rsmc C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(rsmc SHARED
        src/rsmc.c
        include/rsmc.h
)

target_include_directories(rsmc PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

file(STRINGS "include/rsmc/version.h" RSMC_MAJOR_LINE REGEX "^[ \t]*#define[ \t]+RSMC_VERSION_MAJOR")
file(STRINGS "include/rsmc/version.h" RSMC_MINOR_LINE REGEX "^[ \t]*#define[ \t]+RSMC_VERSION_MINOR")
file(STRINGS "include/rsmc/version.h" RSMC_PATCH_LINE REGEX "^[ \t]*#define[ \t]+RSMC_VERSION_PATCH")

if (RSMC_MAJOR_LINE)
    string(REGEX MATCH "RSMC_VERSION_MAJOR[ \t]+([0-9]+)" _ ${RSMC_MAJOR_LINE})
    set(RSMC_VERSION_MAJOR ${CMAKE_MATCH_1})
else ()
    message(FATAL_ERROR "Major version not found in include/rsmc/version.h")
endif ()
if (RSMC_MAJOR_LINE)
    string(REGEX MATCH "RSMC_VERSION_MINOR[ \t]+([0-9]+)" _ ${RSMC_MINOR_LINE})
    set(RSMC_VERSION_MINOR ${CMAKE_MATCH_1})
else ()
    message(FATAL_ERROR "Minor version not found in include/rsmc/version.h")
endif ()
if (RSMC_MAJOR_LINE)
    string(REGEX MATCH "RSMC_VERSION_PATCH[ \t]+([0-9]+)" _ ${RSMC_PATCH_LINE})
    set(RSMC_VERSION_PATCH ${CMAKE_MATCH_1})
else ()
    message(FATAL_ERROR "Patch version not found in include/rsmc/version.h")
endif ()

set_target_properties(rsmc PROPERTIES
        VERSION "${RSMC_VERSION_MAJOR}.${RSMC_VERSION_MINOR}.${RSMC_VERSION_PATCH}"
        SOVERSION "${RSMC_VERSION_MAJOR}"
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
endif ()

option(RSMC_BENCHMARK "Bench rmsc on your hardware" ${PROJECT_IS_TOP_LEVEL})

if (RSMC_BENCHMARK)
    add_subdirectory(benchmark)
endif ()
