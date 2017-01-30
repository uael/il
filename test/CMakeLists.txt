set(TESTS jayl-test)
set(TESTS_F32 jayl-test-f32)

file(GLOB GTEST_SOURCE gtest/gtest-all.cc)
file(GLOB GTEST_HEADER gtest/gtest.h)
include_directories(${JAYL_TEST_DIR})
add_library(gtest ${GTEST_HEADER} ${GTEST_SOURCE})

file(GLOB HEADERS *.h)
file(GLOB SOURCES *.cc)

add_executable(${TESTS} ${SOURCES} ${HEADERS})
target_link_libraries(${TESTS} gtest)
target_link_libraries(${TESTS} pthread)
target_link_libraries(${TESTS} ${PROJECT_NAME})

add_executable(${TESTS_F32} ${SOURCES})
target_link_libraries(${TESTS_F32} gtest)
target_link_libraries(${TESTS_F32} pthread)
set_target_properties(${TESTS_F32} PROPERTIES COMPILE_DEFINITIONS F32)
target_link_libraries(${TESTS_F32} ${PROJECT_NAME})

set(JAYL_TEST_INPUT_DIR ${JAYL_TEST_DIR}/input)
add_definitions(-DTEST_INPUT_DIR="${JAYL_TEST_INPUT_DIR}")
add_definitions(-DAPPS_DIR="${JAYL_APPS_DIR}")
