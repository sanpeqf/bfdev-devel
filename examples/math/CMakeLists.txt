# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
#

add_executable(math-abs abs.c)
target_link_libraries(math-abs bfdev)
add_test(math-abs math-abs)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        abs.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/list
    )

    install(TARGETS
        math-abs
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
