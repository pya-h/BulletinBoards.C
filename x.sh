#!/bin/bash
echo Compiling ...
gcc main.c app/bulletin-boards.c lib/base.c lib/user.c lib/board.c lib/link-list.c
echo Done.
./a.out
