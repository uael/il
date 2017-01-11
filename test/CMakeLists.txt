enable_testing()

include_directories(${gtest_SOURCE_DIR}/include)

set(check_SOURCES
        argument.cc
        function.cc
        frame.cc)

add_executable(check main.cc ${check_SOURCES})
target_link_libraries(check gtest gtest_main parse ast cgen)

add_test(check check)

add_subdirectory(hw)