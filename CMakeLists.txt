cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
project(il)

cmake_policy(SET CMP0054 OLD)
cmake_policy(SET CMP0045 OLD)

option(COVERAGE "Turn on COVERAGE support" OFF)

if (NOT COMMAND add_vendor)
  function(add_vendor)
    include(CMakeParseArguments)
    cmake_parse_arguments(DL_ARGS "" "PROJ" "" ${ARGN})
    set(DL_ARGS_BUILD_DIR "${CMAKE_BINARY_DIR}/vendor/${DL_ARGS_PROJ}")
    set(DL_ARGS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/vendor/${DL_ARGS_PROJ}")
    file(WRITE "${DL_ARGS_BUILD_DIR}/CMakeLists.txt"
      "cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)\n"
      "project(vendor-${DL_ARGS_PROJ} NONE)\n"
      "include(ExternalProject)\n"
      "ExternalProject_Add(vendor-${DL_ARGS_PROJ}\n"
      "${DL_ARGS_UNPARSED_ARGUMENTS}\nSOURCE_DIR \"${DL_ARGS_SOURCE_DIR}\"\n"
      "CONFIGURE_COMMAND \"\"\nBUILD_COMMAND \"\"\nINSTALL_COMMAND \"\"\nTEST_COMMAND \"\")")
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}"
      -D "CMAKE_MAKE_PROGRAM:FILE=${CMAKE_MAKE_PROGRAM}" .
      RESULT_VARIABLE result OUTPUT_QUIET
      WORKING_DIRECTORY "${DL_ARGS_BUILD_DIR}")
    if (result)
      message(FATAL_ERROR "Config step for ${DL_ARGS_PROJ} failed: ${result}")
    endif ()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
      RESULT_VARIABLE result OUTPUT_QUIET
      WORKING_DIRECTORY "${DL_ARGS_BUILD_DIR}")
    if (result)
      message(FATAL_ERROR "Build step for ${DL_ARGS_PROJ} failed: ${result}")
    endif ()
    add_subdirectory(${DL_ARGS_SOURCE_DIR})
  endfunction()
endif ()

if ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
  set(CLANG TRUE)
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "GNU")
  set(GCC TRUE)
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "Intel")
  set(ICC TRUE)
elseif (NOT MSVC)
  message(FATAL_ERROR "Unknown compiler")
endif ()

if (CMAKE_VERSION VERSION_LESS "3.1")
  if (GCC)
    set(CMAKE_C_FLAGS "-std=c99 ${CMAKE_C_FLAGS}")
  endif ()
else ()
  set(CMAKE_C_STANDARD 99)
endif ()

if (COVERAGE AND NOT MSVC)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage")
endif ()

set(${PROJECT_NAME}_HEADERS)
set(${PROJECT_NAME}_SOURCES)
set(${PROJECT_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(${PROJECT_NAME}_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(${PROJECT_NAME}_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)
set(${PROJECT_NAME}_VENDOR_DIR ${CMAKE_CURRENT_LIST_DIR}/vendor)

if (EXISTS ${${PROJECT_NAME}_INCLUDE_DIR})
  file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/*.h)
endif ()
if (EXISTS ${${PROJECT_NAME}_SOURCE_DIR})
  file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_SOURCE_DIR}/*.c)
endif ()

if (EXISTS ${${PROJECT_NAME}_VENDOR_DIR}/CMakeLists.txt)
  add_subdirectory(${${PROJECT_NAME}_VENDOR_DIR})
endif ()
if (EXISTS ${${PROJECT_NAME}_SOURCE_DIR}/CMakeLists.txt)
  add_subdirectory(${${PROJECT_NAME}_SOURCE_DIR})
endif ()

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${${PROJECT_NAME}_HEADERS}")

add_dependencies(${PROJECT_NAME} u)
target_link_libraries(${PROJECT_NAME} u)

if (EXISTS ${${PROJECT_NAME}_INCLUDE_DIR})
  target_include_directories(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_INCLUDE_DIR})
endif ()
if (EXISTS ${${PROJECT_NAME}_SOURCE_DIR})
  target_include_directories(${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_SOURCE_DIR})
endif ()

if (MSVC)
  target_compile_options(${PROJECT_NAME}
    PRIVATE /Oy
    PRIVATE /O$<$<CONFIG:Debug>:d>$<$<CONFIG:Release>:x>)
else ()
  target_compile_options(${PROJECT_NAME}
    PRIVATE -Wall -Werror -Wextra -fomit-frame-pointer
    PRIVATE -O$<$<CONFIG:Debug>:0 -g3>$<$<CONFIG:Release>:3>)
endif ()

if (MSVC)
  set(CMAKE_FLAGS
    CMAKE_C_FLAGS CMAKE_CXX_FLAGS
    CMAKE_C_FLAGS_DEBUG CMAKE_CXX_FLAGS_DEBUG
    CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELEASE)
  foreach (CMAKE_FLAG ${CMAKE_FLAGS})
    string(REPLACE "/MD" "/MT" ${CMAKE_FLAG} "${${CMAKE_FLAG}}")
    string(REPLACE "/MDd" "/MTd" ${CMAKE_FLAG} "${${CMAKE_FLAG}}")
  endforeach ()
endif ()

get_directory_property(${PROJECT_NAME}_DEVEL PARENT_DIRECTORY)
if (NOT ${PROJECT_NAME}_DEVEL)
  if (EXISTS ${${PROJECT_NAME}_TEST_DIR}/CMakeLists.txt)
    enable_testing()
    add_vendor(PROJ cute
      GIT_REPOSITORY https://github.com/uael/cute.git
      GIT_TAG master)
    add_subdirectory(${${PROJECT_NAME}_TEST_DIR})
  endif ()

  install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/${PROJECT_NAME})
  if (EXISTS ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}.h)
    install(FILES ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}.h
      DESTINATION include)
  endif ()
else ()
  set(${PROJECT_NAME}_HEADERS ${PROJECT_NAME}_HEADERS PARENT_SCOPE)
  set(${PROJECT_NAME}_SOURCES ${PROJECT_NAME}_SOURCES PARENT_SCOPE)
  set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_NAME}_INCLUDE_DIR PARENT_SCOPE)
  set(${PROJECT_NAME}_SOURCE_DIR ${PROJECT_NAME}_SOURCE_DIR PARENT_SCOPE)
  set(${PROJECT_NAME}_TEST_DIR ${PROJECT_NAME}_TEST_DIR PARENT_SCOPE)
  set(${PROJECT_NAME}_VENDOR_DIR ${PROJECT_NAME}_VENDOR_DIR PARENT_SCOPE)
endif ()
