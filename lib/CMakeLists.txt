include(ExternalProject)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(ENV{CFLAGS} ${DEBUG_FLAGS})
    set(libfirm_VARIANT "debug")
else()
    set(libfirm_VARIANT "optimize")
endif()

set(libfirm_PREFIX firm)
set(libfirm_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/${libfirm_PREFIX})
set(libfirm_BINARY_DIR ${CMAKE_BINARY_DIR}/lib/${libfirm_PREFIX})
set(libfirm_INSTALL_DIR ${CMAKE_BINARY_DIR}/libfirm)

ExternalProject_Add(libfirm
    SOURCE_DIR "${libfirm_SOURCE_DIR}"
    BINARY_DIR "${libfirm_BINARY_DIR}"
    INSTALL_DIR "${libfirm_INSTALL_DIR}"
    CONFIGURE_COMMAND ""
    PREFIX "${libfirm_PREFIX}"
    BUILD_COMMAND $(MAKE)
        -C "${libfirm_SOURCE_DIR}"
        "variant=${libfirm_VARIANT}"
        "top_builddir=${libfirm_BINARY_DIR}"
    INSTALL_COMMAND $(MAKE)
        -C "${libfirm_SOURCE_DIR}"
        "variant=${libfirm_VARIANT}"
        "top_builddir=${libfirm_BINARY_DIR}"
        "PREFIX=/"
        "DESTDIR=${libfirm_INSTALL_DIR}"
        "install"
)

set(jayl_DEPS ${jayl_DEPS} libfirm PARENT_SCOPE)
set(jayl_LDADD ${jayl_LDADD} ${libfirm_INSTALL_DIR}/lib/libfirm.so m PARENT_SCOPE)
set(jayl_INCLUDES ${jayl_INCLUDES} ${libfirm_INSTALL_DIR}/include PARENT_SCOPE)
set(jayl_DEFS ${jayl_DEFS} -DWITH_LIBFIRM PARENT_SCOPE)
