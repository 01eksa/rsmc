add_executable(rsmc_test rsmc_test.c)
target_link_libraries(rsmc_test PRIVATE rsmc unity)
add_test(NAME rsmc_test COMMAND rsmc_test)

if(WIN32)
    # Windows has no rpath equivalent -- the loader only looks beside the
    # .exe, in system dirs, or on PATH. rsmc.dll lives in a different
    # build-tree directory (multi-config generator puts each target under
    # its own subfolder), so copy it next to rsmc_test.exe after linking.
    add_custom_command(TARGET rsmc_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:rsmc_test>
            $<TARGET_FILE_DIR:rsmc_test>
        COMMAND_EXPAND_LISTS
    )
endif()
