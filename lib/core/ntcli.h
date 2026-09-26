/**
 * @file ntcli.h
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

#ifndef NTCLI_H
#define NTCLI_H

#include "ntcmd.h"
#include "ntshell.h"

/**
 * @brief The command line interface handler.
 * @details
 * A high level facade on top of the ntcmd command framework and the ntshell
 * line editor. It owns the command table and an embedded ntshell handler, so
 * the whole interactive shell is driven by ntcli_init / ntcli_loop.
 */
typedef struct {
    const ntcmd_tbl_t *cmds;    /**< The command table. */
    int count;                  /**< The number of entries. */
    ntshell_t ntsh;             /**< The embedded NT-Shell handler. */
} ntcli_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the command line interface.
 * @details
 * Registers the command table and initializes the embedded NT-Shell. The
 * serial read/write functions are passed through to the NT-Shell, and the
 * enter / TAB completion callbacks are wired to ntcli_func / ntcli_complete.
 *
 * @param cli A pointer to the handler.
 * @param func_read Serial read function.
 * @param func_write Serial write function.
 * @param cmds The command table.
 * @param count The number of entries.
 */
void ntcli_init(ntcli_t *cli,
        NTSHELL_SERIAL_READ func_read,
        NTSHELL_SERIAL_WRITE func_write,
        const ntcmd_tbl_t *cmds, int count);

/**
 * @brief Run the command line interface loop.
 * @details Sets the prompt and runs the loop of the embedded NT-Shell.
 * Never returns. Note that NT-Shell drops the return value of the enter
 * callback, so a command cannot ask the shell to exit by its return code.
 * An "exit" command must terminate by itself (exit(), system reset, ...).
 *
 * @param cli A pointer to the handler.
 * @param prompt The prompt text string.
 */
void ntcli_loop(ntcli_t *cli, const char *prompt);

#ifdef __cplusplus
}
#endif

#endif
