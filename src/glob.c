/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
 */

#include <base.h>
#include <bfdev/glob.h>
#include <export.h>

export bool
bfdev_glob(const char *patten, const char *string)
{
    const char *class, *bpatten, *bstring;
    char ptch, stch, tcha, tchb;
    bool match, inverted;

    bpatten = NULL;
    bstring = NULL;

    for (;;) {
        ptch = *patten++;
        stch = *string++;

        switch (ptch) {
            case '?':
                if (stch == '\0')
                    return false;
                break;

            case '*':
                if (*patten == '\0')
                    return true;
                bpatten = patten;
                bstring = --string;
                break;

            case '[':
                match = false;
                inverted = *patten == '!';
                class = patten + inverted;
                tcha = *class++;

                do {
                    tchb = tcha;
                    if (tcha == '\0')
                        goto literal;

                    if (class[0] == '-' && class[1] != ']') {
                        tchb = class[1];
                        if (tchb == '\0')
                            goto literal;
                        class += 2;
                    }
                    match |= tcha <= stch && stch <= tchb;
                } while ((tcha = *class++) != ']');

                if (match == inverted)
                    goto backtrack;

                patten = class;
                break;

            case '\\':
                ptch = *patten++;
                bfdev_fallthrough;

            default: literal:
                if (ptch == stch) {
                    if (ptch == '\0')
                        return true;
                    break;
                }

            backtrack:
                if (stch == '\0' || !bpatten)
                    return false;

                patten = bpatten;
                string = ++bstring;
                break;
        }
    }
}
