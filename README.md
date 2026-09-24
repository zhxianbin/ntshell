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
