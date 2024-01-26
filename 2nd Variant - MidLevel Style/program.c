#include <boards.h>
#include <lists.h>
#include <tasks.h>
#include <stdio.h>

void help() {
    puts("\nProgram command-lines (Commands are not Case-Sensitive):\n");
    puts("Show this guide: help\n");
    puts("Login: lg {username} {password}\n");
    puts("Register: rg {username} {password}\n");
    puts("Show current Board/List/Task Menu: show"); // if just a board is selected, board menu will be shown
// if a list is selected then list menu will be shown, and etc.
    puts("Select Board: add br {board name}");
    puts("Select Board: sel br {id}");

// after board sleected; otherwise shows an error
    puts("Select Board: add ls {list name}");
    puts("Select List: sel ls {id}"); 

// after board and list are selected; otherwise shows an error
    puts("Select Board: add ts {task name} {deadline} {priority}"); 
    puts("Select Task: sel ts {id}");

}

int main() {
    int flag_is_command_ine = 1; // 1 => user can use command lines
    // 0 => show menu; use must select menu
    
}
