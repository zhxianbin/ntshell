# NT-Shell native sample

A host (Linux) sample for NT-Shell. No cross toolchain is required.

## Build & run

```sh
make
./ntshell_native
```

## What it demonstrates

- A raw terminal (`termios`) is used for the serial read/write functions.
- A command table with `help`, `info sys` / `info ver`, `echo`, `exit`.
- **TAB completion**:
  - First word is completed against the command names (`h` + TAB -> `help`).
  - Sub command of `info` is completed (`info s` + TAB -> `info sys`).
  - Press TAB repeatedly to cycle through the candidates.
- The completion source is provided by the user application through the
  `func_suggest` parameter of `ntshell_init`, while the line editing and
  candidate cycling are handled by the library.

## Keys

- `TAB` : complete / next candidate
- `Up` / `Down` (or Ctrl+P / Ctrl+N) : history
- `Left` / `Right` : move cursor
- `Backspace` / `Delete` : delete
- `Ctrl+C` : cancel the current line
