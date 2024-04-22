/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _LOCAL_PORT_LOG_H_
#define _LOCAL_PORT_LOG_H_

#include <base.h>
#include <bfdev/log.h>

#ifndef __INSIDE_LOG__
# error "please don't include this file directly"
#endif

static inline void
generic_log_write(bfdev_log_message_t *msg)
{
    bfport_file *file;

    if (msg->level > BFDEV_LEVEL_WARNING)
        file = bfport_stdout;
    else {
        file = bfport_stderr;
        bfport_fflush(bfport_stdout);
    }

    bfport_fwrite(msg->data, msg->length, 1, file);
}

#endif /* _LOCAL_PORT_LOG_H_ */
