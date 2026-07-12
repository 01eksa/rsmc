add_executable(rsmc_test rsmc_test.c)
target_link_libraries(rsmc_test PRIVATE rsmc unity)
add_test(NAME rsmc_test COMMAND rsmc_test)