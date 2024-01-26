#include "manager.h" // & models.h && stdio.h & string.h
#include "windows.h"
#include "sys/stat.h"

void close_menu() {
    HANDLE handle;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cells;
    COORD coord = {0, 0};

    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle == INVALID_HANDLE_VALUE)
        return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo(handle, &csbi))
        return;
    cells = csbi.dwSize.X * csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
            handle,
            (TCHAR)' ',
            cells,
            coord,
            &count))
        return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
            handle,
            csbi.wAttributes,
            cells,
            coord,
            &count))
        return;

    /* Move the cursor home */
    SetConsoleCursorPosition(handle, coord);
}

void show_help() {
    printf("\nProgram inprintf (Upper or lower case in input commands is not important):\n");
    printf("Show this guide: HELP\n");
    printf("Show current data: SHOW"); /* If just a board is selected, that board lists options will be shown
                                        If a list is selected then that list task options will be shown.
                                        If nothing is selected then board options will be shown.*/
    printf("Add new item: ADD"); // Add current state item => for example if a board is selected then add new list
    printf("Select item: SEL [index]"); // select current state item
    printf("Save changes: SAVE"); //save any change made
}


int main() {
    char command[10], param1[50], param2[50];
    char *input_uppercase;
    struct board *board = NULL;
    struct list *list = NULL;
    struct user *user = NULL;
    printf("WELCOME :)\n");
    while(1) { // program main loop
        if(user == NULL) { // No input other than register and logout work here
            printf("If you want to start using this program, you must login or register first:\n");
            printf("Login: LOG [username] [password]\n");
            printf("Register: REG [username] [password]\n");
            while(user == NULL) { // get login input until no user found
                printf("> "); 
                scanf("%s %s %s", command, param1, param2);// Get input;
                input_uppercase = strupr(command);
                if(!strcmp(input_uppercase, "LOG")) { // if command is LOG
                    
                } else if(!strcmp(input_uppercase, "REG")) { // if command is REG
                    int user_folder = get_user_folder(param1);
                    if(user_folder == EOF) {
                        printf("New user\n");
                    } else {
                        printf("Error: username exists! try again ... ");
                    }
                }
            }
            close_menu();
        } else {
            //printf("");
        }
        getchar();
    }
}
