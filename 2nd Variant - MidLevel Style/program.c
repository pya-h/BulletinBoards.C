#include "manager.h" // & models.h && stdio.h & string.h
#include "windows.h"
#include "sys/stat.h"
#include "stdio.h"

void close_menu() {
    HANDLE handle;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cells;
    COORD coord = {0, 0};
    printf("Press enter to continue ...");
    getchar();
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

void show_help(int user_menu) {
    if(user_menu) {
        printf("If you want to start using this program, you must login or register first:\n");
        printf("Login: LOG [username] [password]\n");
        printf("Register: REG [username] [password]\n");
    } else {
        printf("\nProgram inprintf (Upper or lower case in input commands is not important):\n");
        printf("Show this guide: HELP\n");
        printf("Show current data: SHOW"); /* If just a board is selected, that board lists options will be shown
                                            If a list is selected then that list task options will be shown.
                                            If nothing is selected then board options will be shown.*/
        printf("Add new item: ADD"); // Add current state item => for example if a board is selected then add new list
        printf("Select item: SEL [index]"); // select current state item
        printf("Save changes: SAVE"); //save any change made
    }
}

void show_error(int error_code) {
    printf("* Error: ");
    switch(error_code) {
    case errno_folder_creation_failed:
        printf("Program can not allocate user data!\n");
        break;
    case errno_user_list_not_available:
        printf("Program can not access user list!\n");
        break;
    default:
        printf("Program can not determine what caused error.\n");
        break;
    };
}

char *get_command() {
    char command[10], param1[50], param2[30];
    printf("> ");
    scanf("%s", command); // Process command before getting other parameters,
    return strupr(command);
}

int main() {
    char *command, param1[50], param2[30];
    char *input_uppercase;
    struct board *board = NULL;
    struct list *list = NULL;
    struct user *user = NULL;
    printf("WELCOME :)\n");
    while(1) { // Program main loop
        if(user == NULL) { // No input other than register and logout work here
            int user_folder;
            show_help(1); // Show login/register commands help
            while(user == NULL) { // get login input until no user found
                command = get_command();
                if(!strcmp(input_uppercase, "LOG")) { // if command is LOG
                    scanf("%s %s", param1, param2); // Get input;
                    user_folder = get_user_folder(param1);
                    if(user_folder != EOF) {
                            
                    } printf("No such user found.\n");
                } else if(!strcmp(input_uppercase, "REG")) { // if command is REG
                    scanf("%s %s", param1, param2); // Get input;
                    user_folder = get_user_folder(param1);
                    if(user_folder == EOF) {
                        user_folder = register_user(param1, param2); // Register new user, the result is folder/error number.
                        if(user_folder) { // if everything was ok
                            printf("Registeration done! You can now login as %s.\n", param1);
                        } else show_error(user_folder);
                    } else {
                        printf("Error: username exists! try again ... ");
                    }
                } else if(!strcmp(input_uppercase, "SHOW"))
                    show_help(1);
                else printf("Command not found.\n");
            }
            close_menu();
        } else {
            command = get_command();
        }
        getchar();
    }
}
