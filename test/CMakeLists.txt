# Copyright (c) 2016-2017 Lucas Abel <www.github.com/uael>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, version 2.1.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Lesser Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

file(GLOB ctest_SOURCES ${ctest_SOURCES} ${CMAKE_CURRENT_LIST_DIR}/*.c)

foreach(ctest_src ${ctest_SOURCES})
    get_filename_component(ctest_name ${ctest_src} NAME_WE)
    add_executable(test_${ctest_name} ${ctest_src})
    add_dependencies(test_${ctest_name} ${PROJECT_NAME} cute)
    target_link_libraries(test_${ctest_name} ${PROJECT_NAME} cute)
    add_test(${ctest_name} test_${ctest_name})
endforeach()
