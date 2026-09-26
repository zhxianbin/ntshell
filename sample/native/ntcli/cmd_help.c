/**
 * @file cmd_help.c
 * @author Xianbin Zhang
 *
 * ===============================================================
 * Natural Tiny Shell (NT-Shell) native sample - ntcli
 * ===============================================================
 * Copyright (c) 2010-2016 Shinichiro Nakamura
 * Copyright (c) 2026 Xianbin Zhang
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include "usrcmd.h"
#include "usrcmd_priv.h"

static int cmd_help(int argc, char *const argv[]);

static const char help_text[] =
    "help\n"
    "    - print all commands\n"
    "help command ...\n"
    "    - print detailed help of 'command'\n"
    "?\n"
    "    - alias for help\n";

const ntcmd_tbl_t g_cmd_help NTCLI_CMD = { "help", cmd_help, help_text, 0 };
const ntcmd_tbl_t g_cmd_question NTCLI_CMD = { "?", cmd_help, help_text, 0 };

static int cmd_help(int argc, char *const argv[])
{
    const ntcmd_tbl_t *cmds = usrcmd_get_cmdlist();
    const int count = usrcmd_get_cmdlist_count();
    int i;

    if (argc == 1) {
        for (i = 0; i < count; i++) {
            printf("- %s\r\n", cmds[i].name);
        }
    } else {
        for (i = 1; i < argc; i++) {
            const ntcmd_tbl_t *c =
                ntcmd_find_from_tbl(argv[i], cmds, count);
            if ((c != NULL) && (c->help != NULL)) {
                printf("%s\r\n", c->help);
            } else {
                printf("Unknown command '%s' - try 'help'\r\n", argv[i]);
            }
        }
    }
    return NTCMD_RET_SUCCESS;
}
