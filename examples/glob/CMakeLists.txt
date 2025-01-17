# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_executable(glob utils.c)
target_link_libraries(glob bfdev)

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        utils.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/glob
    )

    install(TARGETS
        glob
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
