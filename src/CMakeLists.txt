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
set(jayl_INCLUDES ${jayl_INCLUDES})
set(jayl_DEPS ${jayl_DEPS})
set(jayl_LDADD ${jayl_LDADD})
set(jayl_DEFS ${jayl_DEFS})

list(REMOVE_ITEM jayl_SOURCES ./main.c)

set(jayl_INCLUDES ${jayl_INCLUDES} .)

include_directories(${jayl_INCLUDES})

add_library(${PROJECT_NAME}so ${jayl_SOURCES} ${jayl_HEADERS})
add_dependencies(${PROJECT_NAME}so ${jayl_DEPS})
target_link_libraries(${PROJECT_NAME}so ${jayl_LDADD})
target_compile_definitions(${PROJECT_NAME}so PRIVATE ${jayl_DEFS})

add_executable(${PROJECT_NAME} ./main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}so)
target_compile_definitions(${PROJECT_NAME} PRIVATE ${jayl_DEFS})
