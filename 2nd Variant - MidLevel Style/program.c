#include "manager.h" // & models.h && stdio.h & string.h
#include "windows.h"
#include "sys/stat.h"
#include "stdio.h"

int next_page(int page, struct board *board, struct list *list, struct task *task) {
    // First clean the page
    HANDLE handle;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cells;
    COORD coord = {0, 0};

    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle == INVALID_HANDLE_VALUE)
        return page;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo(handle, &csbi))
        return page;
    cells = csbi.dwSize.X * csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
            handle,
            (TCHAR)' ',
            cells,
            coord,
            &count))
        return page;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
            handle,
            csbi.wAttributes,
            cells,
            coord,
            &count))
        return page;

    /* Move the cursor home */
    SetConsoleCursorPosition(handle, coord);

    page = page < 4 ? page + 1 : 1; // go to next page, if its at tasks page and next_page is called it returns to All boards page.
    switch(page) {// Printf current page name:
        case 1:
            printf("* All Boards\n\n");
            break;
        case 2:
            printf("Board: %s\n\n", board->name);
            break;
        case 3:
            printf("Board: %s, List: %s\n\n", board->name, list->name);
            break;
        case 4:
            printf("Board: %s, List: %s, Task: %s\n\n", board->name, list->name, task->name);
            break;
        default:
            printf("Invalid Page!\n");
            break;
    }
    return page;
}

void show_help(int page) {
    if(page == 0) {
        printf("If you want to start using this program, you must login or register first:\n");
        printf("Login: LOG [username] [password]\n");
        printf("Register: REG [username] [password]\n");
    } else {
        printf("This page commands (Upper or lower case in input commands is not important):\n");
        printf("Show this guide: HELP\n");
        printf("Show current task: SHOW\n"); /* If just a board is selected, that board lists options will be shown
                                            If a list is selected then that list task options will be shown.
                                            If nothing is selected then board options will be shown.*/
        printf("Select item: SEL [index]\n"); // select current state item
        if(page == 3) {
            printf("Add new task: ADD [task_name]\n"); // Add current state item => for example if a board is selected then add new list with the .name as [name]
            printf("    Then set parameters: [priority] [deadline_year] [deadline_month] [deadline_day]\n");
        } else if(page == 4) {
            printf("Select task: EDIT -N {new_name}\n"); // edit the name of task
            printf("Select task: EDIT -P {new_priority}\n"); // edit the name of task
            printf("Select task: EDIT -D {new_year} {new_month} {new_day}\n"); // edit the name of task
        } else {
            printf("Add new board/list: ADD [name]\n"); // Add current state item => for example if a board is selected then add new list with the .name as [name]
            printf("Edit board/list: EDIT {new_name}\n");
        }
        if(page > 1 && page <= 4)
            printf("Remove item: REM\n");
        printf("Save changes: SAVE\n"); //save any change made
        printf("Previous page: BACK\n"); //save any change made
        
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
        case errno_task_prioirty_invalid:
            printf("Priority can only be 0, 1 or 2\n");
            break;
        case errno_task_data_invalid:
            printf("Enter a valid date as [year] [month] [day]\n");
            break;
        default:
            printf("Program can not determine what caused error.\n");
            break;
    };
}

char *get_command() {
    char command[10];
    printf("> ");
    scanf("%s", command); // Process command before getting other parameters,
    return strupr(command);
}

int check_page(int *page, struct board *board, struct list *list, struct task *task) {
    /* Check if page number is right, If not show error and reset page
    If its ok show proper message*/
    switch(*page) {
        case 0: // Login/Register page
        case 1:
            // Nothing to check
            break;
        case 2:
            if(board == NULL) {
                printf("Error: No board selected; You must select a board.\n");
                (*page)--;
                return 0; // 0 = page number was wrong
            }
            break;
        case 3:
            if(list == NULL) {
                printf("Error: No list selected; You must select a list.\n");
                (*page)--;
                return 0;
            }
            break;
        case 4:
            if(task == NULL) {
                printf("Error: No task selected; You must select a task.\n");
                (*page)--;
                return 0;
            }
            break;
        default:
            printf("Error: Program is in a wrong state! Now you will sent to boards page.\n");
            *page = 1;
            return 0;
    }
    return 1;
}

int main() {
    char *command;
    struct board *board = NULL;
    struct list *list = NULL;
    struct task *task = NULL;
    struct user *user = NULL;

    /* The program will save into file manualy by SAVE command
        This is for when user makes mistakes */
    int unsaved_changes = 0; 

    int page = 0; // 0 login/register

    printf("WELCOME :)\n");
    while(1) { // Program main loop
        if(user == NULL) { // No input other than register and logout work here
            int user_folder;
            char username[50], password[30];
            show_help(page); // Show login/register commands help
            while(user == NULL) { // get login input until no user found
                command = get_command();
                if(!strcmp(command, "LOG")) { // if command is LOG
                    scanf("%s %s", username, password); // Get input;
                    user_folder = get_user_folder(username);
                    if(user_folder > 0) { // if user found
                        user = login_user(user_folder, username, password);

                        if(!user) {
                            printf("username or password is invalid.");
                        }
                    } else printf("No such user found.\n");

                } else if(!strcmp(command, "REG")) { // if command is REG
                    scanf("%s %s", username, password); // Get input;
                    user_folder = get_user_folder(username);
                    if(user_folder == EOF) {
                        user_folder = register_user(username, password); // Register new user, the result is folder/error number.
                        if(user_folder) { // if everything was ok
                            printf("Registeration done! You can now login as %s.\n", username);
                        } else show_error(user_folder);
                    } else {
                        printf("Error: username exists! try again ... ");
                    }
                } else if(!strcmp(command, "HELP"))
                    show_help(page);
                else printf("Command not found.\n");
            }
            page = next_page(page, board, list, task);
            
        } else {
            char name[500]; 
            int number;
            if(check_page(&page, board, list, task)) { // Check and correct page number until its ok then get input
                command = get_command();
                if(!strcmp(command, "HELP"))
                    show_help(page);
                else if(!strcmp(command, "ADD")) {
                    scanf("%499[^\n]", name);
                    if(page == 1) { // 1 = board page
                        if(user->my_boards == NULL) {
                            user->my_boards = init_boards(name);
                            printf("Your First board added. Congragulations!\n");
                            unsaved_changes = 1;
                        } else if(add_board(user->my_boards, name)) {
                            printf("Board added.\n");
                            unsaved_changes = 1;
                        } else printf("Error: Board can not add. Porgram load problem!\n");

                    } else if(page == 2) { // 2 == lists page
                        if(board->my_lists == NULL) {
                            board->my_lists = init_lists(name);
                            printf("Your first list added to the board.\n");
                            unsaved_changes = 1;
                        } else if(add_list(board->my_lists, name)) {
                            printf("List added.\n");
                            unsaved_changes = 1;
                        } else printf("Error: List can not add. Board load problem!\n");

                    } else if(page == 3 || page == 4) { // 3 = tasks page
                        int priority, year, month, day;
                        int input_has_error;
                        printf("Parameters: ");
                        scanf("%d %d %d %d", &priority, &year, &month, &day);
                        input_has_error = check_task_input(priority, year, month, day);
                        if(!input_has_error) {
                            if(list->my_tasks == NULL) {
                                list->my_tasks = init_tasks(name, priority, year, month, day);
                                printf("Your first task added to the list.\n");
                            } else if(add_task(list->my_tasks, name, priority, year, month, day)) {
                                printf("Task added.\n");
                                unsaved_changes = 1;
                            } else printf("Error: Task can not add. List load problem!\n");
                        } else show_error(input_has_error);
                    }
                } else if(!strcmp(command, "SEL")) {
                    scanf("%d", &number);
                    if(page == 1) {//select board
                        board = get_board(user->my_boards, number);
                        if(board != NULL)
                            page = next_page(page, board, list, task);
                        else printf("Error: Board not found.\n");
                    } else if(page == 2) {
                        list = get_list(board->my_lists, number);
                        if(board != NULL)
                            page = next_page(page, board, list, task);
                        else printf("Error: List not found.\n");
                    } else if(page == 3) {
                        task = get_task(list->my_tasks, number);
                        if(task != NULL)
                            page = next_page(page, board, list, task);
                        else printf("Error: Task not found.\n");
                    }
                    
                } else if(!strcmp(command, "EDIT")) {
                    if(page == 2) {//select board
                        scanf("%499[^\n]", name);
                        if(board != NULL) {
                            sprintf(board->name, "%s", name);
                            unsaved_changes = 1;
                            printf("Board name changed.\n");
                        } else printf("Error: No board selected.\n");
                    } else if(page == 3) {
                        scanf("%499[^\n]", name);
                        if(list != NULL) {
                            sprintf(list->name, "%s", name);
                            unsaved_changes = 1;
                            printf("List name changed.\n");
                        } else printf("Error: No list selected.\n");
                    } else if(page == 4) {
                        char edit_type[5];
                        if(task != NULL) {
                            scanf("%s", edit_type);
                            if(edit_type[0] == '-' && !edit_type[2]) {
                                char param = edit_type[1];
                                if(param == 'N' || param == 'n') {
                                    scanf("%499[^\n]", name);
                                    sprintf(task->name, "%s", name);
                                    printf("Task name changed.\n");
                                    unsaved_changes = 1;
                                } else if(param == 'P' || param == 'p') {
                                    int priority, input_has_error;
                                    scanf("%d", &priority);
                                    input_has_error = check_task_input(priority, task->date.year, task->date.month, task->date.day); // A test date just sent. The purpose is just checking priority
                                    // I used task->date because its validated before and its not invalid. so only ne pririty will be checked
                                    if(!input_has_error) {
                                        task->priority = priority;
                                        printf("Task priority changed.\n");
                                        unsaved_changes = 1;
                                    } printf("Error: Priority is invalid. Enter a number between 0 to 2.\n");
                                } else if(param == 'D' || param == 'd') {
                                    int year, month, day, input_has_error;
                                    scanf("%d %d %d", &year, &month, &day);
                                    input_has_error = check_task_input(task->priority, 2024, 1, 1); // A task->date is always valid. The purpose is just checking date
                                    task->date.year = year;
                                    task->date.month = month;
                                    task->date.day = day;
                                    printf("Task deadline changed.\n");
                                    unsaved_changes = 1;
                                } else printf("Error: Task edit parameter is invalid. Valid paramaters: -N, -P, -D\n");

                            } else printf("Error: Task edit parameter is invalid. Valid paramaters: -N, -P, -D\n");
                        } else printf("Error: No task is selected.\n");
                    }
                    
                } else if(!strcmp(command, "SHOW")) {
                    if(page == 1) //select board
                        show_boards(user->my_boards);
                    else if(page == 2)
                        show_lists(board->my_lists);
                    else if(page == 3)
                        show_tasks(list->my_tasks);
                    else if(page == 4)
                        show_single_task(task);
                } else if(!strcmp(command, "BACK")) {
                    if(page == 1) {
                        printf("This is the first page.\n");
                    } else if(page > 1 && page < 5) {
                        task = NULL;
                        if(page <= 3)
                            list = NULL;
                        if(page == 2)
                            board = NULL;
                        page--;
                        next_page(page - 1, board, list, task); // this function increase page value, so page - 1 is used
                    } else {
                        // invalid page;
                        page = 1;
                    }
                } else if(!strcmp(command, "REM")) {
                    char sure[5], answer;
                    printf("Are you sure? Y=Yes Other=No :");
                    if(page == 2)
                        printf("\n  * All of it\'s lists and tasks will remove too. ");
                     if(page == 3)
                        printf("\n  * All of it\'s tasks will remove too. ");
                    scanf("%s", sure);
                    answer = sure[0];
                    if(answer == 'y' || answer == 'Y') {
                        if(page == 2) {
                            int result = remove_board(user, board);
                            if(result != 1) show_error(result);
                            else {
                                printf("Board deleted.\n");
                                task = NULL;
                                list = NULL;
                                board = NULL;
                                page--; // go to previous page
                                next_page(page - 1, board, list, task); // this function increase page value, so page - 1 is used
                            }
                        } else if(page == 3) {
                            int result = remove_list(board, list);
                            if(result != 1) show_error(result);
                            else {
                                printf("List deleted.\n");
                                list = NULL;
                                task = NULL;
                                page--; // go to previous page
                                next_page(page - 1, board, list, task); // this function increase page value, so page - 1 is used
                            }
                        } else if(page == 4) {
                            int result = remove_task(list, task);
                            if(result != 1) show_error(result);
                            else {
                                printf("Task deleted.\n");
                                task = NULL;
                                page--; // go to previous page
                                next_page(page - 1, board, list, task); // this function increase page value, so page - 1 is used
                            }
                        }
                    } else printf("Removing canceled.\n");
                }
            }
        }   
    }
}