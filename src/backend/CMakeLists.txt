file(GLOB jayl_HEADERS ${jayl_HEADERS} *.h)
file(GLOB jayl_SOURCES ${jayl_SOURCES} *.c)

add_subdirectory(graphviz)
add_subdirectory(x86_64)

set(jayl_SOURCES ${jayl_SOURCES} PARENT_SCOPE)
set(jayl_HEADERS ${jayl_HEADERS} PARENT_SCOPE)
