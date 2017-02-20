set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-missing-braces -Wno-psabi")

set(jayl_SOURCE_DIRS . util)

foreach(dir ${jayl_SOURCE_DIRS})
    file(GLOB jayl_HEADERS ${jayl_HEADERS} ${dir}/*.h)
    file(GLOB jayl_SOURCES ${jayl_SOURCES} ${dir}/*.c)
endforeach()

add_subdirectory(adt)
add_subdirectory(backend)
add_subdirectory(frontend)
add_subdirectory(lower)

set(jayl_HEADERS ${jayl_HEADERS})
set(jayl_SOURCES ${jayl_SOURCES})

# list(REMOVE_ITEM jayl_SOURCES ./main.c)

include_directories(.)

add_executable(${PROJECT_NAME} ${jayl_SOURCES} ${jayl_HEADERS})
add_dependencies(${PROJECT_NAME} libfirm)
target_link_libraries(${PROJECT_NAME} ${libfirm_LIBRARIES})
