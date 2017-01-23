set(jay_SOURCES
        sentence.cc
        grammar.cc
        jayl.cc
        jay.cc)

add_executable(jay ${jay_SOURCES})
target_link_libraries(jay parse ast cgen)