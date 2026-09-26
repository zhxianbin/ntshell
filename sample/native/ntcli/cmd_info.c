/**
 * @file cmd_info.c
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
#include "usrcmd_priv.h"

#define SUBCOUNT(t) \
    ((int)(sizeof(t) / sizeof(t[0])))

static int cmd_info(int argc, char *const argv[]);
static int cmd_info_complete(int argc, char *const argv[], int maxv,
        const char *cmdv[]);
static int info_sys(int argc, char *const argv[]);
static int info_ver(int argc, char *const argv[]);

static const char info_text[] =
    "info sys\n"
    "    - show the system name\n"
    "info ver\n"
    "    - show the version\n";

/**
 * @brief Sub-command table of "info".
 */
static const ntcmd_tbl_t g_info_sub_cmds[] = {
    { "sys", info_sys, "info sys - show the system name", 0 },
    { "ver", info_ver, "info ver - show the version", 0 },
};

const ntcmd_tbl_t g_cmd_info NTCLI_CMD =
    { "info", cmd_info, info_text, cmd_info_complete };

static int cmd_info(int argc, char *const argv[])
{
    const ntcmd_tbl_t *c;

    argc--;
    argv++;
    if (argc < 1) {
        return NTCMD_RET_USAGE;
    }

    c = ntcmd_find_from_tbl(argv[0], g_info_sub_cmds, SUBCOUNT(g_info_sub_cmds));
    if (c != NULL) {
        return c->func(argc, argv);
    }
    return NTCMD_RET_USAGE;
}

static int cmd_info_complete(int argc, char *const argv[], int maxv,
        const char *cmdv[])
{
    return ntcmd_match_args(g_info_sub_cmds, SUBCOUNT(g_info_sub_cmds),
            argc, argv, maxv, cmdv);
}

static int info_sys(int argc, char *const argv[])
{
    (void)argc;
    (void)argv;
    printf("Native Linux host\r\n");
    return NTCMD_RET_SUCCESS;
}

static int info_ver(int argc, char *const argv[])
{
    (void)argc;
    (void)argv;
    printf("NT-Shell native sample v0.2.0\r\n");
    return NTCMD_RET_SUCCESS;
}
