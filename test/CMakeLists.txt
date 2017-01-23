enable_testing()

include_directories(${gtest_SOURCE_DIR}/include)

set(check_SOURCES
        001.cc)

add_executable(check main.cc ${check_SOURCES})
target_link_libraries(check gtest gtest_main)

add_test(check check)