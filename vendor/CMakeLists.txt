include(ExternalProject)

set(libfirm_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/firm)
set(libfirm_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/firm)

add_subdirectory(firm)
add_subdirectory(ltalloc)

set(jayl_DEPS ${jayl_DEPS} firm ltalloc PARENT_SCOPE)
if(WIN32)
    set(jayl_LDADD ${jayl_LDADD} firm ltalloc winmm PARENT_SCOPE)
elseif(UNIX)
    set(jayl_LDADD ${jayl_LDADD} firm ltalloc m PARENT_SCOPE)
endif()
set(jayl_INCLUDES ${jayl_INCLUDES} ${libfirm_SOURCE_DIR}/include)
set(jayl_INCLUDES ${jayl_INCLUDES} ${libfirm_BINARY_DIR}/gen/include)
set(jayl_INCLUDES ${jayl_INCLUDES} ${libfirm_BINARY_DIR}/gen/include/libfirm)
set(jayl_INCLUDES ${jayl_INCLUDES} ${CMAKE_CURRENT_LIST_DIR}/ltalloc PARENT_SCOPE)
set(jayl_DEFS ${jayl_DEFS} -DWITH_LIBFIRM -DWITH_LTALLOC PARENT_SCOPE)
