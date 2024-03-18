# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_compile_options(
    -std=gnu11
    -Wall
    -Wextra
    -Wno-override-init
    -Wno-unused-parameter
    -Wno-sign-compare
    -Wno-pointer-sign
    -Wno-null-pointer-arithmetic
    -Wmissing-prototypes
    -Wmissing-declarations
    -fvisibility=hidden
)

if(BFDEV_STRICT)
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} \
         -Werror"
    )
endif()

if(BFDEV_ASAN)
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} \
         -fsanitize=address \
         -fsanitize=undefined \
         -fsanitize-recover=all \
         -fno-omit-frame-pointer \
         -fno-stack-protector"
    )
    if(NOT APPLE)
        set(CMAKE_C_FLAGS
            "${CMAKE_C_FLAGS} \
             -fsanitize=leak"
        )
    endif()
endif()

if(BFDEV_UBSAN)
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} \
         -fsanitize=alignment \
         -fsanitize=bounds \
         -fsanitize=shift \
         -fsanitize=integer-divide-by-zero \
         -fsanitize=unreachable \
         -fsanitize=bool \
         -fsanitize=enum \
         -fsanitize-undefined-trap-on-error"
    )
endif()

if(BFDEV_GCOV)
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} \
         -fprofile-arcs \
         -ftest-coverage"
    )
endif()

asm_generic(
    bfdev/asm-generic/
    ${BFDEV_GENERATED_PATH}/bfdev/asm
    ${BFDEV_ARCH_HEADER_PATH}/bfdev/asm
    ${BFDEV_HEADER_PATH}/bfdev/asm-generic
)

configure_file(
    ${BFDEV_MODULE_PATH}/config.h.in
    ${BFDEV_GENERATED_PATH}/bfdev/config.h
)

configure_file(
    ${BFDEV_MODULE_PATH}/bfdev-config.cmake.in
    ${BFDEV_CONFIGURE}
)

file(GLOB_RECURSE BFDEV_HEADER
    ${BFDEV_HEADER_PATH}/*.h
)

file(GLOB_RECURSE BFDEV_GENERATED_HEADER
    ${BFDEV_GENERATED_PATH}/*.h
)

file(GLOB_RECURSE BFDEV_ARCH_HEADER
    ${BFDEV_ARCH_HEADER_PATH}/*.h
)

set(BFDEV_INCLUDE_DIRS
    ${BFDEV_HEADER_PATH}
    ${BFDEV_GENERATED_PATH}
    ${BFDEV_ARCH_HEADER_PATH}
)

set_property(
    GLOBAL PROPERTY
    "BFDEV_INCLUDE_DIRS"
    ${BFDEV_INCLUDE_DIRS}
)

include_directories(${BFDEV_INCLUDE_DIRS})
include(${BFDEV_ARCH_PATH}/build.cmake)
include(${BFDEV_SOURCE_PATH}/build.cmake)

set(BFDEV_LIBRARY_HEADER
    ${BFDEV_HEADER}
    ${BFDEV_ASM_HEADER}
    ${BFDEV_ARCH_ASM_HEADER}
    ${BFDEV_GENERATED_HEADER}
)

set(BFDEV_LIBRARY_SOURCE
    ${BFDEV_SOURCE}
    ${BFDEV_ARCH_SOURCE}
)

set(BFDEV_LIBRARY
    ${BFDEV_LIBRARY_HEADER}
    ${BFDEV_LIBRARY_SOURCE}
)
