# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

macro(packed_header prefix name genfile source)
    file(GLOB srclist ${source}/*.h)

    file(REMOVE ${genfile})
    file(WRITE ${genfile}
        "/*\n"
        " * Automatically generated file; DO NOT EDIT.\n"
        " * " ${CMAKE_PROJECT_NAME} " pack-header\n"
        " */\n"
        "\n"
        "#ifndef " ${name} "\n"
        "#define " ${name} "\n"
        "\n"
    )

    foreach(srcpath ${srclist})
        string(REGEX REPLACE ".+/(.+)" "\\1" filename ${srcpath})
        file(APPEND ${genfile}
            "#include <" ${prefix} ${filename} ">\n"
        )
    endforeach()

    file(APPEND ${genfile}
        "\n"
        "#endif /* " ${name} " */\n"
    )

    unset(srclist)
endmacro()
