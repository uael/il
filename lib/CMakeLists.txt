include(ExternalProject)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(ENV{CFLAGS} ${DEBUG_FLAGS})
    set(libfirm_VARIANT "debug")
else()
    set(libfirm_VARIANT "optimize")
endif()

set(libfirm_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/firm)
set(libfirm_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/firm)

add_subdirectory(firm)

set(jayl_DEPS ${jayl_DEPS} firm PARENT_SCOPE)
if(WIN32)
    set(jayl_LDADD ${jayl_LDADD} firm winmm PARENT_SCOPE)
elseif(UNIX)
    set(jayl_LDADD ${jayl_LDADD} firm m PARENT_SCOPE)
endif()
set(jayl_INCLUDES ${jayl_INCLUDES} ${libfirm_SOURCE_DIR}/include)
set(jayl_INCLUDES ${jayl_INCLUDES} ${libfirm_BINARY_DIR}/gen/include)
set(jayl_INCLUDES ${jayl_INCLUDES} ${libfirm_BINARY_DIR}/gen/include/libfirm PARENT_SCOPE)
set(jayl_DEFS ${jayl_DEFS} -DWITH_LIBFIRM PARENT_SCOPE)
