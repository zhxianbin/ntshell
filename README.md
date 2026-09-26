# Natural Tiny Shell (NT-Shell)

## What's this?

Natural Tiny Shell (NT-Shell) is a C library for embedded systems.
It provides VT100 compatible terminal control feature and needs only serial read/write functions for the porting.


## Features

- Compatible with VT100 :)
- Really simple.
  - The API has only three functions.
  - It consists of only three small modules.
- Highly portable.
  - Compatible with C89.
  - No dependencies. (even libc!)
  - No dynamic memory allocation. (no need a operating system!)
- Small code foot print.
  - ROM: 10KB
  - RAM: 1KB


## Building with Meson

This project ships a `meson.build` so it can be built standalone or embedded
into another Meson project as a subproject.

### Standalone

```sh
meson setup build -Dsamples=true   # samples are optional
meson compile -C build
```

The native sample binaries end up in `build/sample/native/`.

### Configuration options

The `ntconf.h` limits can be overridden with Meson options
(`-D<name>=<value>` on setup, or `default_options` when used as a
subproject). The values are applied to the library and propagated to
everything that links against `ntshell_dep`.

| Option           | Default | Overrides              |
| ---------------- | ------- | ---------------------- |
| `editor_maxlen`  | 64      | `NTCONF_EDITOR_MAXLEN` |
| `history_depth`  | 8       | `NTCONF_HISTORY_DEPTH` |
| `cli_max_args`   | 10      | `NTCONF_CLI_MAX_ARGS`  |

```sh
meson setup build -Deditor_maxlen=128 -Dhistory_depth=16 -Dcli_max_args=20
```

Without Meson, the same macros can still be defined with plain compiler
flags, e.g. `CFLAGS="-DNTCONF_EDITOR_MAXLEN=128"`.

### As a Meson subproject

Place this repository under `subprojects/ntshell` (or use a wrap file) and:

```meson
ntshell_dep = dependency('ntshell',
  fallback : ['ntshell', 'ntshell_dep'],
  default_options : ['editor_maxlen=128'])
executable('my_app', 'main.c', dependencies : ntshell_dep)
```


## Edit Controls

| Action                                  | Key input             |
| ----------------------------------------| --------------------- |
| Move to the start of line               | CTRL+A or Home        |
| Move to the end of line                 | CTRL+E or End         |
| Move forward one character              | CTRL+F or Right arrow |
| Move back one character                 | CTRL+B or Left arrow  |
| Delete previous character               | Backspace             |
| Delete current character                | CTRL+D or Delete      |
| Cancel current input line               | CTRL+C                |
| History search (backward)               | CTRL+P                |
| History search (forward)                | CTRL+N                |
| Input suggestion from history record    | TAB                   |


----

Natural Tiny Shell (NT-Shell)

Copyright (C) 2010-2017 Shinichiro Nakamura (CuBeatSystems)

- https://twitter.com/shintamainjp
- http://shinta-main-jp.blogspot.jp/
- https://cubeatsystems.com/ntshell/index.html
