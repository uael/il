set(jay_SOURCES
        jay.cc)

add_executable(jay ${jay_SOURCES})
target_link_libraries(jay parse)