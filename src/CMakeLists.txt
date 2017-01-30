# Jayl
set(CMAKE_MACOSX_RPATH 1)

if (JAYL_SHARED_LIBRARY)
    set(JAYL_LIBRARY_TYPE SHARED)
    message("-- Shared library")
else ()
    set(JAYL_LIBRARY_TYPE STATIC)
    message("-- Static library")
endif ()

set(JAYL_SOURCE_DIRS . interfaces util)

foreach (dir ${JAYL_SOURCE_DIRS})
    file(GLOB JAYL_HEADERS ${JAYL_HEADERS} ${dir}/*.h)
    file(GLOB JAYL_SOURCES ${JAYL_SOURCES} ${dir}/*.cc)
endforeach ()
list(REMOVE_ITEM JAYL_SOURCES ${CMAKE_CURRENT_LIST_DIR}/main.cc)

set(JAYL_HEADERS ${JAYL_HEADERS})
set(JAYL_SOURCES ${JAYL_SOURCES})
set(JAYL_SOURCES_NO_RTTI "")

add_subdirectory(backend)
add_subdirectory(frontend)
add_subdirectory(lower)

add_definitions(${JAYL_DEFINITIONS})
include_directories(${JAYL_INCLUDE_DIRS})
add_library(${PROJECT_NAME} ${JAYL_LIBRARY_TYPE} ${JAYL_HEADERS} ${JAYL_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${JAYL_LIBRARIES})
add_executable(jay main.cc)
target_link_libraries(jay ${PROJECT_NAME})

# Handle no RTTI sources
set_source_files_properties(${JAYL_SOURCES_NO_RTTI} PROPERTIES COMPILE_FLAGS "-fno-rtti")
