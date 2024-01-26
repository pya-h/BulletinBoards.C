#include "my_models.h"
#include "stdio.h"
#include <windows.h>
#include <windows.h>

void close_menu() {
    HANDLE hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = {0, 0};

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE)
        return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))
        return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
            hStdOut,
            (TCHAR)' ',
            cellCount,
            homeCoords,
            &count))
        return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
            hStdOut,
            csbi.wAttributes,
            cellCount,
            homeCoords,
            &count))
        return;

    /* Move the cursor home */
    SetConsoleCursorPosition(hStdOut, homeCoords);
}

void show_help() {
    puts("\nProgram command-lines (Commands are not Case-Sensitive):\n");
    puts("Show this guide: help\n");
    puts("Login: lg {username} {password}\n");
    puts("Register: rg {username} {password}\n");
    puts("Show current Board/List/Task Menu: show"); // if just a board is selected, board menu will be shown
                                                     // if a list is selected then list menu will be shown, and etc.
    puts("Select Board: add board {board name}");
    puts("Select Board: sel board {id}");

    // after board sleected; otherwise shows an error
    puts("Select Board: add list {list name}");
    puts("Select List: sel list {id}");

    puts("Add Task: add list {task name} {deadline} {priority}"); // only if a list is selected
    puts("Select Task: sel task {id}");
}


int main()
{
    int flag_is_command_ine = 1; // 1 => user can use command lines  0 => show menu; use must select menu
    char input[500];

}
