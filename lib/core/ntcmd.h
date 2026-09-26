/**
 * @file ntcmd.h
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

#ifndef NTCMD_H
#define NTCMD_H


/**
 * @brief Command return codes.
 */
enum {
    NTCMD_RET_SUCCESS = 0,    /**< The command finished successfully. */
    NTCMD_RET_USAGE   = 1,    /**< Usage error. */
    NTCMD_RET_FAILURE = 2,    /**< The command failed. */
    NTCMD_RET_NOTFOUND = -1,  /**< The command name was not found. */
};

/**
 * @brief Command table entry.
 * @details
 * The table is owned by the application and passed to the ntcmd_xxx
 * functions as a (pointer, count) pair. The storage of the table (static
 * array, linker section, dynamically allocated block, ...) is an application
 * concern.
 */
typedef struct {
    const char *name;       /**< Command name. */
    int (*func)(int argc, char *const argv[]);  /**< Command function. */
    const char *help;       /**< Detailed help text. May be NULL. */
    /**
     * @brief Completion hook.
     * @details
     * Fills cmdv with up to maxv candidate strings and returns the number of
     * candidates stored in cmdv (never more than maxv). Unlike a line editor
     * that inserts a suffix at the cursor, NT-Shell replaces the whole last
     * word, so every candidate is a full word. argv[0] is the command name
     * and argv[1..] are the arguments. May be NULL.
     */
    int (*complete)(int argc, char *const argv[], int maxv, const char *cmdv[]);
} ntcmd_tbl_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Find a command in a table.
 * @details
 * Supports a length modifier after a dot (e.g. "cp.b") and a unique
 * abbreviation (e.g. "inf" matches "info" when unambiguous).
 *
 * @param cmd The command name (possibly abbreviated or with a modifier).
 * @param table The command table.
 * @param table_len The number of entries.
 *
 * @return A pointer to the entry, or NULL if not found or ambiguous.
 */
const ntcmd_tbl_t *ntcmd_find_from_tbl(const char *cmd,
        const ntcmd_tbl_t *table, int table_len);

/**
 * @brief Match candidates for the last word of an argument vector.
 * @details
 * argv[0] is a dummy ("fakeroot") and argv[1..] are the parsed words. Fills
 * cmdv with the candidate full words and returns the number of candidates
 * stored in cmdv.
 *
 * @param table The command table.
 * @param table_len The number of entries.
 * @param argc The number of arguments including the dummy.
 * @param argv The arguments including the dummy.
 * @param maxv The capacity of cmdv.
 * @param cmdv The candidate buffer.
 *
 * @return The number of candidates stored in cmdv (at most maxv).
 */
int ntcmd_match_args(const ntcmd_tbl_t *table, int table_len,
        int argc, char *const argv[], int maxv, const char *cmdv[]);

/**
 * @brief Execute a command by its argument vector.
 *
 * @param table The command table.
 * @param table_len The number of entries.
 * @param argc The number of arguments.
 * @param argv The arguments.
 *
 * @return The command return value, or NTCMD_RET_NOTFOUND.
 */
int ntcmd_execute_args(const ntcmd_tbl_t *table, int table_len,
        int argc, char *const argv[]);

#ifdef __cplusplus
}
#endif

#endif
