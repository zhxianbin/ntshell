/**
 * @file cmd_exit.c
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
#include <stdlib.h>
#include "usrcmd_priv.h"

static int cmd_exit(int argc, char *const argv[]);

static const char exit_text[] =
    "exit\n"
    "    - exit the shell\n";

const ntcmd_tbl_t g_cmd_exit NTCLI_CMD = { "exit", cmd_exit, exit_text, 0 };

static int cmd_exit(int argc, char *const argv[])
{
    (void)argc;
    (void)argv;
    printf("bye\r\n");
    exit(0);
    return NTCMD_RET_SUCCESS;
}
