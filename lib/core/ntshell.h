/**
 * @file ntshell.h
 * @author CuBeatSystems
 * @author Shinichiro Nakamura
 * @copyright
 * ===============================================================
 * Natural Tiny Shell (NT-Shell) Version 0.3.1
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

#ifndef NTSHELL_H
#define NTSHELL_H

#include "vtrecv.h"
#include "vtsend.h"
#include "text_editor.h"
#include "text_history.h"

#define NTSHELL_PROMPT_MAXLEN   (32)
#define NTSHELL_PROMPT_DEFAULT  ">"
#define NTSHELL_PROMPT_NEWLINE  "\r\n"

typedef int (*NTSHELL_SERIAL_READ)(char *buf, int cnt, void *extobj);
typedef int (*NTSHELL_SERIAL_WRITE)(const char *buf, int cnt, void *extobj);
typedef int (*NTSHELL_USER_CALLBACK)(const char *text, void *extobj);

/**
 * @brief Callback for the tab completion.
 * @details
 * Get the index-th candidate text string whose prefix matches the given text.
 * This function is called repeatedly with an increasing index while the
 * user presses the TAB key, until it returns a non-zero value.
 *
 * @param text The current text string.
 * @param index The index number of the candidate.
 * @param buf A pointer to the buffer for the candidate.
 * @param siz A size of the buffer.
 * @param extobj An external object.
 *
 * @retval 0 Found a candidate. The candidate is written to the buffer.
 * @retval !0 Not found.
 */
typedef int (*NTSHELL_USER_SUGGEST_CALLBACK)(const char *text, int index, char *buf, int siz, void *extobj);

/**
 * @brief The handler of NT-Shell.
 * @details
 * The best way to provide a handler, We should hide the implementation from the library users.
 * But some small embedded environments can not use memory allocation dynamically.
 * So this implementation open the fields of the handler in this header.
 * You can use this handler on the stacks.
 */
typedef struct {
    unsigned int initcode;  /**< Initialization flag. */
    vtsend_t vtsend;        /**< The handler of vtsend. */
    vtrecv_t vtrecv;        /**< The handler of vtrecv. */
    text_editor_t editor;   /**< The handler of text_editor. */
    text_history_t history; /**< The handler of text_history. */
    int suggest_index;
    char suggest_source[TEXTEDITOR_MAXLEN];
    NTSHELL_SERIAL_READ func_read;
    NTSHELL_SERIAL_WRITE func_write;
    NTSHELL_USER_CALLBACK func_callback;
    NTSHELL_USER_SUGGEST_CALLBACK func_suggest;
    void *extobj;
    char prompt[NTSHELL_PROMPT_MAXLEN];
} ntshell_t;

#ifdef __cplusplus
extern "C" {
#endif

void ntshell_init(ntshell_t *p,
    NTSHELL_SERIAL_READ func_read,
    NTSHELL_SERIAL_WRITE func_write,
    NTSHELL_USER_CALLBACK func_callback,
    NTSHELL_USER_SUGGEST_CALLBACK func_suggest,
    void *extobj);
void ntshell_execute(ntshell_t *p);
void ntshell_set_prompt(ntshell_t *p, const char *prompt);
void ntshell_version(int *major, int *minor, int *release);

#ifdef __cplusplus
}
#endif

#endif

