@echo off
echo Compiling ...
gcc main.c app/bulletin-boards.c lib/base.c lib/user.c lib/board.c lib/link-list.c lib/task-list.c lib/task.c
echo Done.
.\a.exe
