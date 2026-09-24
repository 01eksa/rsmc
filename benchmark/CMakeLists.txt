add_executable(rsmcbench rsmc_benchmark.c)
target_link_libraries(rsmcbench PRIVATE rsmc)

add_compile_definitions(_POSIX_C_SOURCE=199309L)

if(WIN32)
    # Windows has no rpath equivalent -- the loader only looks beside the
    # .exe, in system dirs, or on PATH. rsmc.dll lives in a different
    # build-tree directory (multi-config generator puts each target under
    # its own subfolder), so copy it next to rsmc_test.exe after linking.
    add_custom_command(TARGET rsmcbench POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:rsmcbench>
            $<TARGET_FILE_DIR:rsmcbench>
            COMMAND_EXPAND_LISTS
    )
endif()
