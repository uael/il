include(ExternalProject)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(ENV{CFLAGS} ${DEBUG_FLAGS})
    set(libfirm_VARIANT "debug")
else()
    set(libfirm_VARIANT "optimize")
endif()

set(libfirm_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/firm)
set(libfirm_BINARY_DIR ${libfirm_SOURCE_DIR}/build)

ExternalProject_Add(libfirm
    SOURCE_DIR "${libfirm_SOURCE_DIR}"
    BINARY_DIR "${libfirm_BINARY_DIR}"
    INSTALL_DIR "${CMAKE_BINARY_DIR}/libfirm"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND $(MAKE)
        -C "${libfirm_SOURCE_DIR}"
        "variant=${libfirm_VARIANT}"
        "top_builddir=${libfirm_BINARY_DIR}"
    INSTALL_COMMAND $(MAKE)
        -C "${libfirm_SOURCE_DIR}"
        "variant=${libfirm_VARIANT}"
        "top_builddir=${libfirm_BINARY_DIR}"
        "PREFIX=/"
        "DESTDIR=${CMAKE_BINARY_DIR}/libfirm"
        "install"
)

set(libfirm_LIBRARIES "-lfirm -lm")

include_directories(${libfirm_SOURCE_DIR}/include ${libfirm_BINARY_DIR}/include)

add_subdirectory(gtest)
