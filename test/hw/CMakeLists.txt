project(hw)

function(add_jay_executable exec_name)
    set(jay_exec_SOURCES)
    foreach(jay_file ${ARGN})
        string(REPLACE ".jay" ".c" output ${jay_file})
        add_custom_command(
            OUTPUT ${output}
            COMMAND ${CMAKE_BINARY_DIR}/src/jay ${CMAKE_CURRENT_LIST_DIR}/${jay_file}
            DEPENDS ${jay_file}
        )
        list(APPEND jay_exec_SOURCES ${output})
    endforeach()
    add_executable(${exec_name} ${jay_exec_SOURCES})
endfunction()

set(hw_SOURCES
        hw.jay)

add_jay_executable(hw ${hw_SOURCES})