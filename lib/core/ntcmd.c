/**
 * @file ntcmd.c
 * @author Xianbin Zhang
 *
 * ===============================================================
 * Natural Tiny Shell (NT-Shell) command framework
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

#include "ntcmd.h"
#include "ntlibc.h"

/**
 * @brief Find a command in a table.
 */
const ntcmd_tbl_t *ntcmd_find_from_tbl(const char *cmd,
        const ntcmd_tbl_t *table, int table_len)
{
    const ntcmd_tbl_t *cmdtp;
    const ntcmd_tbl_t *cmdtp_temp = table;
    const char *p;
    int len;
    int n_found = 0;

    if ((cmd == 0) || (table == 0) || (table_len == 0)) {
        return 0;
    }

    /*
     * Some commands allow a length modifier (like "cp.b"); compare the
     * command name only until the first dot.
     */
    p = ntlibc_strchr(cmd, '.');
    len = (p == 0) ? ntlibc_strlen(cmd) : (int)(p - cmd);

    /*
     * Edge cases of the length modifier: an input like "." or ".x" gives
     * a zero compare length, which prefix-matches every entry (and ends up
     * ambiguous unless the table has exactly one entry). Also, a command
     * name containing a dot can never be matched past the dot. Neither is
     * useful, so keep command names dot-free.
     */

    for (cmdtp = table; cmdtp != table + table_len; cmdtp++) {
        if (ntlibc_strncmp(cmd, cmdtp->name, len) == 0) {
            if (len == ntlibc_strlen(cmdtp->name)) {
                return cmdtp;   /* full match */
            }
            cmdtp_temp = cmdtp; /* abbreviated command ? */
            n_found++;
        }
    }

    if (n_found == 1) {
        return cmdtp_temp;      /* exactly one match */
    }

    return 0;                   /* not found or ambiguous command */
}

/**
 * @brief Match candidates for the last word of an argument vector.
 */
int ntcmd_match_args(const ntcmd_tbl_t *table, int table_len,
        int argc, char *const argv[], int maxv, const char *cmdv[])
{
    int i;
    int matches = 0;
    const ntcmd_tbl_t *owner = 0;

    argc--;
    argv++;

    if (argc < 1) {
        /*
         * Empty line: list all command names.
         */
        for (i = 0; (i < table_len) && (matches < maxv); i++) {
            cmdv[matches++] = table[i].name;
        }
    } else {
        owner = ntcmd_find_from_tbl(argv[0], table, table_len);
        if (owner != 0) {
            if (argc == 1) {
                /*
                 * A single word: complete it to the full command name when
                 * it is a (unique) abbreviation.
                 */
                if (ntlibc_strcmp(argv[0], owner->name) != 0) {
                    cmdv[matches++] = owner->name;
                }
            } else if (owner->complete != 0) {
                /*
                 * More than one word: delegate to the command's hook.
                 */
                matches = owner->complete(argc, argv, maxv, cmdv);
            }
        } else {
            /*
             * Not a command: prefix-match against all command names.
             */
            for (i = 0; (i < table_len) && (matches < maxv); i++) {
                if (ntlibc_strncmp(argv[0], table[i].name,
                            ntlibc_strlen(argv[0])) == 0) {
                    cmdv[matches++] = table[i].name;
                }
            }
        }
    }

    return matches;
}

/**
 * @brief Execute a command by its argument vector.
 */
int ntcmd_execute_args(const ntcmd_tbl_t *table, int table_len,
        int argc, char *const argv[])
{
    const ntcmd_tbl_t *cmd;

    if (argc < 1) {
        return NTCMD_RET_SUCCESS;
    }

    cmd = ntcmd_find_from_tbl(argv[0], table, table_len);
    if (cmd == 0) {
        return NTCMD_RET_NOTFOUND;
    }

    return cmd->func(argc, argv);
}
