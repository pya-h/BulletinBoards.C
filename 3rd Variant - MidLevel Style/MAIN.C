#include "CSVFILE.h"
#include <stdlib.h>
#include <stdio.h>

int next_page(int page, struct board *board, struct list *list, struct task *task, int has_unsaved_changes);

int main()
{
    char *UserName = NULL;
    while (1)
    {
        if (UserName == NULL)
        { // Agar Login Nashodeh
            int user_folder;
            char InputUser[1000], Password[1000], Input[1000];
            // Dastorateh Login:
            // login username password
            // register username password
            puts("Please login or register:");
            scanf("%s %s %s", Input, InputUser, Password);

            if (strcmp(Input, "login") == NULL)
            { // if command is LOG
                UserName = login_user(InputUser, Password);
                if (UserName == NULL)
                    puts("Username or Password Incorrect! Try again!\n");
            }
            else if (strcmp(Input, "register") == NULL)
            {                                                 // if command is REG
                UserName = register_user(username, password); // Register new user, the result is folder/error number.
                if (UserName == NULL)
                    puts("Register Error! Try again!\n");
            }
        }
        else
        {
            puts("[S]HOW BOARDS\t[A]DD BOARD\t[E]NTER BOARD")
            if (check_page(&page, board, list, task))
            { // Check and correct page number until its ok then get input
                command = get_command();
                if (!strcmp(command, "HELP"))
                    show_help(page);
                else if (!strcmp(command, "ADD"))
                {
                    scanf("%499[^\n]", name);
                    if (page == 1)
                    { // 1 = board page
                        if (user->my_boards == NULL)
                        {
                            user->my_boards = init_boards(name);
                            printf("Your First board added. Congragulations!\n");
                            unsaved_changes = 1;
                            user->boards_count = 1;
                        }
                        else if (add_board(user->my_boards, name))
                        {
                            printf("Board added.\n");
                            unsaved_changes = 1;
                            user->boards_count++;
                        }
                        else
                            printf("Error: Board can not add. Porgram load problem!\n");
                    }
                    else if (page == 2)
                    { // 2 == lists page
                        if (board->my_lists == NULL)
                        {
                            board->my_lists = init_lists(name);
                            printf("Your first list added to the board.\n");
                            unsaved_changes = 1;
                            board->lists_count = 1;
                        }
                        else if (add_list(board->my_lists, name))
                        {
                            printf("List added.\n");
                            unsaved_changes = 1;
                            board->lists_count++;
                        }
                        else
                            printf("Error: List can not add. Board load problem!\n");
                    }
                    else if (page == 3 || page == 4)
                    { // 3 = tasks page
                        int priority;
                        int year = 0, month = 0, day = 0;
                        int input_has_error;
                        printf("Parameters: ");
                        scanf("%d %d %d %d", &priority, &year, &month, &day);
                        input_has_error = check_task_input(priority, year, month, day);
                        if (!input_has_error)
                        {
                            if (list->my_tasks == NULL)
                            {
                                list->my_tasks = init_tasks(name, priority, year, month, day);
                                printf("Your first task added to the list.\n");
                                unsaved_changes = 1;
                                list->tasks_count = 1;
                            }
                            else if (add_task(list->my_tasks, name, priority, year, month, day))
                            {
                                printf("Task added.\n");
                                list->tasks_count++;
                                unsaved_changes = 1;
                            }
                            else
                                printf("Error: Task can not add. List load problem!\n");
                        }
                        else
                            show_error(input_has_error);
                    }
                }
                else if (!strcmp(command, "SEL"))
                {
                    scanf("%d", &number);
                    if (page == 1)
                    { // select board
                        board = get_board(user->my_boards, number);
                        if (board != NULL)
                            page = next_page(page, board, list, task, unsaved_changes);
                        else
                            printf("Error: Board not found.\n");
                    }
                    else if (page == 2)
                    {
                        list = get_list(board->my_lists, number);
                        if (list != NULL)
                            page = next_page(page, board, list, task, unsaved_changes);
                        else
                            printf("Error: List not found.\n");
                    }
                    else if (page == 3)
                    {
                        task = get_task(list->my_tasks, number);
                        if (task != NULL)
                            page = next_page(page, board, list, task, unsaved_changes);
                        else
                            printf("Error: Task not found.\n");
                    }
                }
                else if (!strcmp(command, "EDIT"))
                {
                    if (page == 2)
                    { // select board
                        scanf("%499[^\n]", name);
                        if (board != NULL)
                        {
                            sprintf(board->name, "%s", name);
                            unsaved_changes = 1;
                            printf("Board name changed.\n");
                        }
                        else
                            printf("Error: No board selected.\n");
                    }
                    else if (page == 3)
                    {
                        scanf("%499[^\n]", name);
                        if (list != NULL)
                        {
                            sprintf(list->name, "%s", name);
                            unsaved_changes = 1;
                            printf("List name changed.\n");
                        }
                        else
                            printf("Error: No list selected.\n");
                    }
                    else if (page == 4)
                    {
                        char edit_type[5];
                        if (task != NULL)
                        {
                            scanf("%s", edit_type);
                            if (edit_type[0] == '-' && !edit_type[2])
                            {
                                char param = edit_type[1];
                                if (param == 'N' || param == 'n')
                                {
                                    scanf("%499[^\n]", name);
                                    sprintf(task->name, "%s", name);
                                    printf("Task name changed.\n");
                                    unsaved_changes = 1;
                                }
                                else if (param == 'P' || param == 'p')
                                {
                                    int priority, input_has_error;
                                    scanf("%d", &priority);
                                    input_has_error = check_task_input(priority, task->date_year, task->date_month, task->date_day); // A test date just sent. The purpose is just checking priority
                                    // I used task->date because its validated before and its not invalid. so only ne pririty will be checked
                                    if (!input_has_error)
                                    {
                                        task->priority = priority;
                                        printf("Task priority changed.\n");
                                        unsaved_changes = 1;
                                    }
                                    printf("Error: Priority is invalid. Enter a number between 0 to 2.\n");
                                }
                                else if (param == 'D' || param == 'd')
                                {
                                    int year = 0, month = 0, day = 0;
                                    int input_has_error;
                                    scanf("%d %d %d", &year, &month, &day);
                                    input_has_error = check_task_input(task->priority, year, month, day); // A task->date is always valid. The purpose is just checking date
                                    if (!input_has_error)
                                    {
                                        task->date_year = year;
                                        task->date_month = month;
                                        task->date_day = day;
                                        printf("Task deadline changed.\n");
                                        unsaved_changes = 1;
                                    }
                                    else
                                        printf("Error: Deadline value invalid.\n");
                                }
                                else
                                    printf("Error: Task edit parameter is invalid. Valid paramaters: -N, -P, -D\n");
                            }
                            else
                                printf("Error: Task edit parameter is invalid. Valid paramaters: -N, -P, -D\n");
                        }
                        else
                            printf("Error: No task is selected.\n");
                    }
                }
                else if (!strcmp(command, "SHOW"))
                {
                    if (page == 1) // select board
                        show_boards(user->my_boards);
                    else if (page == 2)
                        show_lists(board->my_lists);
                    else if (page == 3)
                        show_tasks(list->my_tasks);
                    else if (page == 4)
                        show_single_task(task, 0); // 0 = show no number
                }
                else if (!strcmp(command, "BACK"))
                {
                    if (page == 1)
                    {
                        printf("This is the first page.\n");
                    }
                    else if (page > 1 && page < 5)
                    {
                        task = NULL;
                        if (page <= 3)
                            list = NULL;
                        if (page == 2)
                            board = NULL;
                        page--;
                        next_page(page - 1, board, list, task, unsaved_changes); // this function increase page value, so page - 1 is used
                    }
                    else
                    {
                        // invalid page;
                        page = 1;
                    }
                }
                else if (!strcmp(command, "REM"))
                {
                    char sure[5], answer;
                    printf("Are you sure? Y=Yes Other=No :");
                    if (page == 2)
                        printf("\n  * All of it\'s lists and tasks will remove too. ");
                    if (page == 3)
                        printf("\n  * All of it\'s tasks will remove too. ");
                    scanf("%s", sure);
                    answer = sure[0];
                    if (answer == 'y' || answer == 'Y')
                    {
                        if (page == 2)
                        {
                            int result = remove_board(user, board);
                            if (result != 1)
                                show_error(result);
                            else
                            {
                                printf("Board deleted.\n");
                                task = NULL;
                                list = NULL;
                                board = NULL;
                                page--; // go to previous page
                                unsaved_changes = 1;
                                user->boards_count--;
                                next_page(page - 1, board, list, task, unsaved_changes); // this function increase page value, so page - 1 is used
                            }
                        }
                        else if (page == 3)
                        {
                            int result = remove_list(board, list);
                            if (result != 1)
                                show_error(result);
                            else
                            {
                                printf("List deleted.\n");
                                list = NULL;
                                task = NULL;
                                board->lists_count--;
                                page--; // go to previous page
                                unsaved_changes = 1;

                                next_page(page - 1, board, list, task, unsaved_changes); // this function increase page value, so page - 1 is used
                            }
                        }
                        else if (page == 4)
                        {
                            int result = remove_task(list, task);
                            if (result != 1)
                                show_error(result);
                            else
                            {
                                printf("Task deleted.\n");
                                task = NULL;
                                list->tasks_count--;
                                page--; // go to previous page
                                unsaved_changes = 1;
                                next_page(page - 1, board, list, task, unsaved_changes); // this function increase page value, so page - 1 is used
                            }
                        }
                    }
                    else
                        printf("Removing canceled.\n");
                }
                else if (page == 3 && !strcmp(command, "SORT"))
                {
                    char by[5];
                    scanf("%s", by);
                    if (by[0] == '-')
                    {
                        if (by[1] == 'p' || by[1] == 'P')
                        {
                            sort_list(list, 0);
                            unsaved_changes = 1;
                        }
                        else if (by[1] == 'd' || by[1] == 'D')
                        {
                            sort_list(list, 1);
                            unsaved_changes = 1;
                        }
                        else
                            printf("Error: Invalid sort parameter. [P/D]\n");
                    }
                }
                else if (page == 4 && task != NULL && !strcmp(command, "MOVE"))
                {
                    struct list *target_list;
                    show_lists(board->my_lists);
                    printf("Select the target list: ");
                    scanf("%d", &number);
                    target_list = get_list(board->my_lists, number);
                    if (list != NULL)
                    {
                        int result = move_task(task, list, target_list);
                        if (result != 1)
                            show_error(result);
                        else
                        {
                            printf("Task moved to selected list. You are now in the target list.\n");
                            list->tasks_count--;
                            target_list->tasks_count++;
                            unsaved_changes = 1;
                            list = target_list;
                        }
                    }
                    else
                        printf("Error: List not found.\n");
                }
                else if (!strcmp(command, "SAVE"))
                {
                    int result = save(user);
                    if (result != 1)
                        show_error(result);
                    else
                    {
                        printf("Everything saved.\n");
                        unsaved_changes = 0;
                    }
                }
                else
                    printf("Error: No such command.\n");
            }
        }
    }
}

int next_page(int page, struct board *board, struct list *list, struct task *task, int has_unsaved_changes)
{
    // Paak Kardane Safhe
    system("cls");

    page = page < 4 ? page + 1 : 1; // go to next page, if its at tasks page and next_page is called it returns to All boards page.
    if (has_unsaved_changes)
        printf("*Warning: You have unsaved changes.\n");

    switch (page)
    { // Printf current page name:
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