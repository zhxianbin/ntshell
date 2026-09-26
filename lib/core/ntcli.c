/**
 * @file ntcli.c
 * @author Xianbin Zhang
 *
 * ===============================================================
 * Natural Tiny Shell (NT-Shell) command line interface
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

#include "ntcli.h"
#include "ntlibc.h"
#include "ntconf.h"

/**
 * @brief Maximum number of arguments of cli.
 */
#define CLI_MAX_ARGS    (NTCONF_CLI_MAX_ARGS)

/**
 * @brief Is the character a whitespace delimiter?
 */
static int ntcli_isspace(char c)
{
    return (c == ' ') || (c == '\t') || (c == '\r') || (c == '\n');
}

/**
 * @brief Split a line into an argument vector in place.
 * @details
 * Words beyond maxargs are silently dropped. An input line of
 * NTCONF_EDITOR_MAXLEN characters can hold at most CLI_MAX_ARGS words,
 * so the truncation can only happen with pathological input.
 */
static int ntcli_parse_line(char *line, char *argv[], int maxargs)
{
    int nargs = 0;

    while (nargs < maxargs) {
        while (ntcli_isspace(*line)) {
            line++;
        }
        if (*line == '\0') {
            break;
        }
        argv[nargs++] = line;
        while ((*line != '\0') && !ntcli_isspace(*line)) {
            line++;
        }
        if (*line == '\0') {
            break;
        }
        *line++ = '\0';
    }
    argv[nargs] = 0;
    return nargs;
}

/**
 * @brief Match completion candidates for a command line.
 * @details
 * NT-Shell completion replaces the whole last word, so a trailing whitespace
 * means the word under the cursor is empty. An empty word is appended before
 * matching so the command's completion hook can complete the next argument.
 */
static int ntcli_match_line(const ntcmd_tbl_t *table, int table_len,
        char *cmdline, int maxv, const char *cmdv[])
{
    char *argv[CLI_MAX_ARGS + 2];
    int len = ntlibc_strlen(cmdline);
    int trailing_space = (len > 0) &&
        ((cmdline[len - 1] == ' ') || (cmdline[len - 1] == '\t'));
    int argc;

    argc = ntcli_parse_line(cmdline, &argv[1], CLI_MAX_ARGS);
    argv[0] = "fakeroot";

    if (trailing_space && (argc >= 1) && (argc < CLI_MAX_ARGS)) {
        argv[argc + 1] = (char *)"";
        argc++;
    }

    return ntcmd_match_args(table, table_len, argc + 1, argv, maxv, cmdv);
}

/**
 * @brief The enter callback of the embedded NT-Shell.
 * @details Executes one input line through ntcmd_execute_args, then turns
 * the result code into terminal output: "Unknown command" for
 * NTCMD_RET_NOTFOUND and the help text for NTCMD_RET_USAGE.
 */
static int ntcli_func(const char *text, void *extobj)
{
    ntcli_t *cli = (ntcli_t *)extobj;
    ntshell_t *ntsh = &cli->ntsh;
    char line[NTCONF_EDITOR_MAXLEN];
    char *argv[CLI_MAX_ARGS + 1];
    int argc;
    int ret;

    ntlibc_strcpy(line, text);
    argc = ntcli_parse_line(line, argv, CLI_MAX_ARGS);

    ret = ntcmd_execute_args(cli->cmds, cli->count, argc, argv);
    if (ret == NTCMD_RET_NOTFOUND) {
        ntsh->func_write("Unknown command: ", ntlibc_strlen("Unknown command: "), ntsh->extobj);
        ntsh->func_write(argv[0], ntlibc_strlen(argv[0]), ntsh->extobj);
        ntsh->func_write(" - try 'help'\r\n", ntlibc_strlen(" - try 'help'\r\n"), ntsh->extobj);
    } else if (ret == NTCMD_RET_USAGE) {
        const ntcmd_tbl_t *cmd =
            ntcmd_find_from_tbl(argv[0], cli->cmds, cli->count);
        if ((cmd != 0) && (cmd->help != 0)) {
            ntsh->func_write(cmd->help, ntlibc_strlen(cmd->help), ntsh->extobj);
        }
        ntsh->func_write("\r\n", ntlibc_strlen("\r\n"), ntsh->extobj);
    }
    return ret;
}

/**
 * @brief The tab completion callback of the embedded NT-Shell.
 * @details The semantics follows NTSHELL_USER_SUGGEST_CALLBACK.
 */
static int ntcli_complete(const char *text, int index, char *buf, int siz, void *extobj)
{
    ntcli_t *cli = (ntcli_t *)extobj;
    char line[NTCONF_EDITOR_MAXLEN];
    const char *cmdv[CLI_MAX_ARGS];
    int i;
    int matches;
    int last_word_start;
    const char *cand;

    ntlibc_strcpy(line, text);

    last_word_start = 0;
    for (i = 0; text[i] != '\0'; i++) {
        if ((text[i] == ' ') || (text[i] == '\t')) {
            last_word_start = i + 1;
        }
    }

    matches = ntcli_match_line(cli->cmds, cli->count, line,
            CLI_MAX_ARGS, cmdv);

    /*
     * ntcmd_match_args never returns more than maxv candidates; the extra
     * index < CLI_MAX_ARGS check protects cmdv from a misbehaving
     * command completion hook.
     */
    if ((index >= matches) || (index >= CLI_MAX_ARGS)) {
        return -1;
    }

    cand = cmdv[index];
    if (siz <= last_word_start + ntlibc_strlen(cand)) {
        return -1;
    }
    for (i = 0; i < last_word_start; i++) {
        buf[i] = text[i];
    }
    ntlibc_strcpy(buf + last_word_start, cand);
    return 0;
}

/**
 * @brief Initialize the command line interface.
 */
void ntcli_init(ntcli_t *cli,
        NTSHELL_SERIAL_READ func_read,
        NTSHELL_SERIAL_WRITE func_write,
        const ntcmd_tbl_t *cmds, int count)
{
    cli->cmds = cmds;
    cli->count = count;
    ntshell_init(&cli->ntsh,
            func_read, func_write, ntcli_func, ntcli_complete, cli);
}

/**
 * @brief Run the command line interface loop.
 * @details Sets the prompt and runs the loop of the embedded NT-Shell.
 * Never returns from this function.
 */
void ntcli_loop(ntcli_t *cli, const char *prompt)
{
    ntshell_set_prompt(&cli->ntsh, prompt);
    ntshell_execute(&cli->ntsh);
}
