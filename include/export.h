/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _EXPORT_H_
#define _EXPORT_H_

#include <bfdev/config.h>
#include <bfdev/attributes.h>

#undef hidden
# define hidden __visibility("hidden")

#undef export
# define export __visibility("default")

#endif /* _EXPORT_H_ */
