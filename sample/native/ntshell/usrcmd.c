/**
 * @file usrcmd.c
 * @author NT-Shell native sample
 *
 * ===============================================================
 * Natural Tiny Shell (NT-Shell) native sample
 * ===============================================================
 * Copyright (c) 2010-2016 Shinichiro Nakamura
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
#include "ntopt.h"
#include "ntlibc.h"
#include "usrcmd.h"

typedef int (*USRCMDFUNC)(int argc, char **argv);

typedef struct {
    const char *cmd;
    const char *desc;
    USRCMDFUNC func;
} cmd_table_t;

static int usrcmd_help(int argc, char **argv);
static int usrcmd_info(int argc, char **argv);
static int usrcmd_echo(int argc, char **argv);
static int usrcmd_exit(int argc, char **argv);

static const cmd_table_t cmdlist[] = {
    { "help", "Show this help.", usrcmd_help },
    { "info", "Show system info. Usage: info sys | info ver", usrcmd_info },
    { "echo", "Echo the arguments back.", usrcmd_echo },
    { "exit", "Exit the shell.", usrcmd_exit },
};

static int usrcmd_ntopt_callback(int argc, char **argv, void *extobj)
{
    unsigned int i;
    (void)extobj;
    if (argc == 0) {
        return 0;
    }
    for (i = 0; i < sizeof(cmdlist) / sizeof(cmdlist[0]); i++) {
        if (ntlibc_strcmp(argv[0], cmdlist[i].cmd) == 0) {
            return cmdlist[i].func(argc, argv);
        }
    }
    printf("Unknown command: %s\r\n", argv[0]);
    return -1;
}

int usrcmd_execute(const char *text)
{
    return ntopt_parse(text, usrcmd_ntopt_callback, 0);
}

/**
 * @brief Tab completion callback.
 * @details
 * The "text" argument is the whole current line. This function completes
 * the last whitespace-delimited word of the line.
 * - First word: completed against the command table.
 * - "info" sub command: completed against the sub command table.
 */
int usrcmd_complete(const char *text, int index, char *buf, int siz, void *extobj)
{
    int word_pos = 0;
    int i, found;
    const char *word;
    (void)extobj;

    /*
     * Find the start of the last word.
     */
    for (i = 0; text[i] != '\0'; i++) {
        if (text[i] == ' ') {
            word_pos = i + 1;
        }
    }
    word = text + word_pos;

    /*
     * Complete the command name.
     */
    if (word_pos == 0) {
        found = 0;
        for (i = 0; i < (int)(sizeof(cmdlist) / sizeof(cmdlist[0])); i++) {
            if (ntlibc_strncmp(cmdlist[i].cmd, word, ntlibc_strlen(word)) == 0) {
                if (found == index) {
                    if (siz <= ntlibc_strlen(cmdlist[i].cmd)) {
                        return -1;
                    }
                    ntlibc_strcpy(buf, cmdlist[i].cmd);
                    return 0;
                }
                found++;
            }
        }
        return -1;
    }

    /*
     * Complete the sub command for "info".
     */
    if ((ntlibc_strncmp(text, "info", 4) == 0) &&
            ((text[4] == ' ') || (text[4] == '\0'))) {
        static const char *sub[] = { "sys", "ver" };
        found = 0;
        for (i = 0; i < (int)(sizeof(sub) / sizeof(sub[0])); i++) {
            if (ntlibc_strncmp(sub[i], word, ntlibc_strlen(word)) == 0) {
                if (found == index) {
                    int j;
                    if (siz <= word_pos + ntlibc_strlen(sub[i])) {
                        return -1;
                    }
                    for (j = 0; j < word_pos; j++) {
                        buf[j] = text[j];
                    }
                    ntlibc_strcpy(buf + word_pos, sub[i]);
                    return 0;
                }
                found++;
            }
        }
        return -1;
    }

    return -1;
}

static int usrcmd_help(int argc, char **argv)
{
    unsigned int i;
    (void)argc;
    (void)argv;
    for (i = 0; i < sizeof(cmdlist) / sizeof(cmdlist[0]); i++) {
        printf("%-8s : %s\r\n", cmdlist[i].cmd, cmdlist[i].desc);
    }
    return 0;
}

static int usrcmd_info(int argc, char **argv)
{
    if (argc != 2) {
        printf("usage:\r\n");
        printf("  info sys\r\n");
        printf("  info ver\r\n");
        return 0;
    }
    if (ntlibc_strcmp(argv[1], "sys") == 0) {
        printf("Native Linux host\r\n");
        return 0;
    }
    if (ntlibc_strcmp(argv[1], "ver") == 0) {
        printf("NT-Shell native sample v0.1.0\r\n");
        return 0;
    }
    printf("Unknown sub command: %s\r\n", argv[1]);
    return -1;
}

static int usrcmd_echo(int argc, char **argv)
{
    int i;
    for (i = 1; i < argc; i++) {
        printf("%s%s", (i > 1) ? " " : "", argv[i]);
    }
    printf("\r\n");
    return 0;
}

static int usrcmd_exit(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    printf("bye\r\n");
    exit(0);
    return 0;
}
