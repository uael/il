enable_testing()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(check_SOURCES
        argument.cc
        function.cc
        frame.cc)

add_executable(check main.cc ${check_SOURCES})

target_link_libraries(check gtest gtest_main)
target_link_libraries(check libdyc)

add_test(check check)