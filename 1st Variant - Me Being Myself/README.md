# Bulletin Boards: 1st Variant

The layered, modular implementation of the project. See the [main README](../README.md) for
how it compares with the other two variants.

## Features

- User registration and login. Passwords are stored obfuscated with a salt; this is not
  cryptographic hashing.
- Create, view, select, rename, and delete **boards**, **lists**, and **tasks**.
- Tasks have a title, a priority (high / medium / low), and a deadline, and each can be edited.
- Move a task from one list to another.
- Deleting a board or list also removes everything inside it, including its data files.
- Numbered menus, with a clear-screen helper that works on every OS.
- All data is persisted as CSV files under `Data/` (one file per user, board, and list).

## Design

```
main.c                 menu loop and screens
app/bulletin-boards.*  application layer: session state (current board/list/task)
lib/link-list.*        generic doubly linked list (void * payloads), reusable in other projects
lib/user.*             registration, login, credential storage
lib/board.*            boards
lib/task-list.*        lists
lib/task.*             tasks (priority, deadline)
lib/base.*             shared macros, string helpers, file/folder helpers, error reporting
```

- Every model is stored in the same generic linked list, so add, find, unlink, and delete are
  written once.
- Each module owns its CSV file format, so the menu code never touches files directly.
- Every item is heap-allocated and freed when it is deleted.

## Build and run (Windows)

```bat
w.bat
```

`w.bat` compiles all sources with GCC (MinGW) and starts the app. `x.sh` is an early Linux
script; the code uses Windows-only headers (`conio.h`, `_mkdir`), so it does not build on Linux.

## Not implemented

- Sorting tasks (implemented in the 2nd variant)
- Reminders
- Logout and automatic re-login

## Known issue

- Moving the only task in a list to another list doesn't work correctly yet.
