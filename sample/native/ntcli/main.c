/**
 * @file main.c
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

/**
 * Native (host) sample for the ntcli API.
 *
 * ntcli owns the command table and the embedded ntshell line editor. The
 * application only provides the serial read/write functions and runs the
 * ntcli_loop.
 *
 * Build and run on Linux:
 *   $ make
 *   $ ./ntshell_native_ntcli
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "ntcli.h"
#include "usrcmd.h"

static struct termios g_orig_tio;
static ntcli_t g_cli;

static void console_init(void)
{
    struct termios tio;

    if (tcgetattr(STDIN_FILENO, &g_orig_tio) != 0) {
        perror("tcgetattr");
        exit(1);
    }

    /*
     * Raw mode without echo. NT-Shell redraws the edited line by itself.
     */
    tio = g_orig_tio;
    tio.c_lflag &= ~(ICANON | ECHO | ISIG);
    tio.c_iflag &= ~(IXON | IXOFF | ICRNL);
    tio.c_cc[VMIN] = 1;
    tio.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tio) != 0) {
        perror("tcsetattr");
        exit(1);
    }
}

static void console_restore(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &g_orig_tio);
}

static int serial_read(char *buf, int cnt, void *extobj)
{
    (void)extobj;
    return (int)read(STDIN_FILENO, buf, (size_t)cnt);
}

static int serial_write(const char *buf, int cnt, void *extobj)
{
    (void)extobj;
    return (int)write(STDOUT_FILENO, buf, (size_t)cnt);
}

int main(void)
{
    console_init();
    /*
     * Restore the terminal when the process exits (e.g. the "exit" command).
     */
    atexit(console_restore);
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("NT-Shell native sample (ntcli)\r\n");
    printf("Type 'help' and press TAB to complete. 'exit' to quit.\r\n");

    /*
     * ntcli registers the command table and initializes the embedded
     * ntshell line editor. ntcli_loop runs the interactive shell.
     */
    ntcli_init(&g_cli,
            serial_read,
            serial_write,
            usrcmd_get_cmdlist(), usrcmd_get_cmdlist_count());
    ntcli_loop(&g_cli, "native>");

    return 0;
}
