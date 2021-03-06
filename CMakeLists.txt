cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
cmake_policy(SET CMP0054 OLD)
cmake_policy(SET CMP0045 OLD)

if (NOT EXISTS ${CMAKE_CACHEFILE_DIR}/cmake)
  execute_process(
    COMMAND git clone https://github.com/uael/cmake-modules.git cmake
    RESULT_VARIABLE result OUTPUT_QUIET
    WORKING_DIRECTORY ${CMAKE_CACHEFILE_DIR}
  )
endif ()

set(CMAKE_MODULE_PATH "${CMAKE_CACHEFILE_DIR}/cmake")
include(ProjectDependency)

project(il C)
project_dependency(ufs GIT_REPOSITORY https://github.com/uael/ufs.git)
project_dependency(uopt GIT_REPOSITORY https://github.com/uael/uopt.git)

set(il_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(il_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(il_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)

file(GLOB_RECURSE il_SRCS ${il_SRCS} ${il_SRC_DIR}/*.c)
file(GLOB_RECURSE il_HDRS ${il_HDRS} ${il_INC_DIR}/il/*.h)
set(il_HDR ${il_INC_DIR}/il.h)

add_library(il ${il_SRCS} ${il_HDRS} ${il_HDR})
target_include_directories(il PUBLIC ${il_INC_DIR})
target_link_libraries(il PUBLIC uds)

get_directory_property(il_PARENT PARENT_DIRECTORY)
if (NOT ${il_PARENT})
  set(il_DEVEL TRUE)
elseif (NOT ${il_DEVEL})
  set(il_DEVEL FALSE)
endif ()
if (${il_DEVEL} AND EXISTS ${il_TEST_DIR})
  if (NOT il_PARENT)
    enable_testing()
  endif ()

  file(GLOB il_TEST_SRCS ${il_TEST_SRCS} ${il_TEST_DIR}/*.c)
  foreach (il_TEST_SRC ${il_TEST_SRCS})
    get_filename_component(il_TEST_NAME ${il_TEST_SRC} NAME_WE)
    add_executable(test_${il_TEST_NAME} ${il_TEST_SRC})
    add_dependencies(test_${il_TEST_NAME} il)
    target_link_libraries(test_${il_TEST_NAME} il)
    add_test(${il_TEST_NAME} test_${il_TEST_NAME})
  endforeach ()
endif ()

install(FILES ${il_HDRS}
  DESTINATION include/il)
install(FILES ${il_HDR}
  DESTINATION include)
