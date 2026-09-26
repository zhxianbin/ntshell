# NT-Shell native-ntcli sample

A host (Linux) sample that uses the high level `ntcli` API.

## What it demonstrates

ntcli mirrors the `cli` + `command` design:

- `lib/core/ntcmd.*` — the command framework:
  - `ntcmd_tbl_t` = `name` / `func` / `help` / optional sub-command `complete`
    hook (returns a candidate list).
  - `ntcmd_find_from_tbl` (lookup; supports a unique abbreviation `inf` ->
    `info` and a length modifier after a dot, `cp.b` style), `ntcmd_match_args`
    (completion), `ntcmd_execute_args` (dispatch). These operate on
    `argc`/`argv` only; line parsing is the `ntcli` layer's concern.
  - Commands return `NTCMD_RET_SUCCESS` / `NTCMD_RET_USAGE` /
    `NTCMD_RET_FAILURE`.
- `lib/core/ntcli.*` — the command line facade:
  - `ntcli_init(cli, func_read, func_write, cmds, count)` — registers the
    command table and initializes the embedded `ntshell_t ntsh` line editor.
    The serial functions are passed through and the enter / TAB callbacks are
    wired to the internal `ntcli_func` / `ntcli_complete`.
  - `ntcli_loop(cli, prompt)` — sets the prompt and runs the interactive
    shell (never returns).
  - `ntcli_func` (enter) parses the line, dispatches via
    `ntcmd_execute_args`, and prints unknown/usage messages;
    `ntcli_complete` (TAB) parses the line, delegates to `ntcmd_match_args`
    via the internal `ntcli_match_line`, and rebuilds the full line.
- The command table is a contiguous array of `ntcmd_tbl_t` entries handed to
  `ntcli_init` as a `(table, count)` pair.
- Each command lives in its own `cmd_*.c` file and places its global
  `ntcmd_tbl_t` variable (e.g. `g_cmd_info`) into a dedicated linker section;
  `usrcmd.c` exposes that section as the command table via the linker-provided
  `__start_`/`__stop_` symbols. `cmd_info.c` also defines the `info sys` /
  `info ver` sub-command table.

The `../ntshell` sample wires `ntshell_init`/`ntshell_execute` directly
and parses commands with `ntopt`; this sample uses the `ntcli` facade
instead.

## Build & run

```sh
make
./ntshell_native_ntcli
```

## Keys

- `TAB` : complete / next candidate (`h` + TAB -> `help`, `info s` + TAB ->
  `info sys`)
- `Up` / `Down` (or Ctrl+P / Ctrl+N) : history
- `Ctrl+C` : cancel the current line
