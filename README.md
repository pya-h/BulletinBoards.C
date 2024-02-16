# Bulletin Boards (C)

A console task manager written in C: users register and log in, then organize their work into
**boards**, each board into **lists**, and each list into **tasks** with a priority and a
deadline. All data is saved to CSV files, so the app works fully offline.

The same specification (a university programming-course project) is implemented here **three
times**, in three deliberately different styles. Each variant has its own architecture, data
structures, naming conventions, file layout, storage format, and user interface. Comparing them
shows how much the same requirements can vary depending on design choices.

## The three variants

| | 1st variant | 2nd variant | 3rd variant |
|---|---|---|---|
| Folder | `1st Variant - Me Being Myself` | `2nd Variant - MidLevel+ Style` | `3rd Variant - MidLevel Style` |
| Style | My own style | Intermediate style | Beginner style |
| Structure | Layered: `lib/` (models + generic linked list) and `app/` (application logic), 8 modules | 3 files: models, manager (storage), program (UI) | 2 files: data module and main |
| Core data structure | Reusable, generic doubly linked list (`void *` payloads) | Hand-rolled singly linked lists inside each struct | Dynamic arrays grown with `realloc` |
| Interface | Numbered menus | Command line: `LOG`, `REG`, `ADD`, `SEL`, `EDIT`, `REM`, `SORT`, `MOVE`, `SAVE`, `BACK`, `HELP` (built-in help screen) | Single-key menus (`[S]how`, `[A]dd`, `[G]oto`, `[E]dit`, `[D]elete`) |
| Storage | One CSV file per user, board, and list under `Data/` | One folder per user with an index file | One shared `DATA.CSV` |
| Naming | `Module_function` (`List_add`) | `snake_case` | `PascalCase` |

### 1st variant (my own style)
The most structured of the three. It has a generic linked-list library (`lib/link-list.h`) that
can be reused in other projects. Boards, lists, tasks, and users are separate modules, and the
app tracks the user's position through a session struct. Errors are reported through a
consistent error-handling layer. Passwords are stored obfuscated with a salt; this is **not**
cryptographic hashing. See its own [README](1st%20Variant%20-%20Me%20Being%20Myself/README.md).

### 2nd variant (intermediate style)
Everything is driven by typed commands instead of menus. It adds sorting a list by priority or
deadline (`SORT -P` / `SORT -D`), moving tasks between lists (`MOVE`), and an unsaved-changes
warning.

### 3rd variant (beginner style)
The simplest design: fixed-size records in arrays that grow with `realloc`, a single CSV file for
all users, and single-key menu navigation.

## Build and run

All three variants target **Windows**: they use `conio.h` / `windows.h` and `_mkdir`, and are
built with GCC (MinGW). From inside a variant's folder:

```bat
w.bat
```

`w.bat` compiles the variant and starts it. The `x.sh` scripts are early Linux attempts and do
not build, because of those Windows-only headers.

## Repository contents

- `project.pdf`: the original project specification (in Persian).
- `1st Variant - Me Being Myself/`, `2nd Variant - MidLevel+ Style/`, `3rd Variant - MidLevel Style/`: the three implementations.
