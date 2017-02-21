file(GLOB jayl_HEADERS ${jayl_HEADERS} *.h)
file(GLOB jayl_SOURCES ${jayl_SOURCES} *.c)

add_subdirectory(jay)

set(jayl_SOURCES ${jayl_SOURCES} PARENT_SCOPE)
set(jayl_HEADERS ${jayl_HEADERS} PARENT_SCOPE)
