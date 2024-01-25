#include "app/bulletin-boards.h"
#include <string.h>

// THINK: write a professional getch() ?
// TODO: this one will count the number of options and gets key until the max length/count
int main()
{
    MenuOption choice;
    Long selectedItemIndex = -1; // choice is of type short and is not suitable with menus with too many items.
    Session session = newSession();
    initializeData();

    while (1)
    {
        session.error = NULL;
        if (session.user != NULL && session.user->loggedIn)
        {
            if (!session.currentBoard)
            {
                choice = boardsMenu();
                switch (choice)
                {
                case MENU_OPTION_CREATE:
                { // because Board is defined inside we used {}
                    Board *nextBoard = createBoardInterface(session.user->id);
                    session.error = Board_getError(nextBoard);
                    if (!session.error) // if board created successfully
                    {
                        List_add(session.boards, nextBoard);
                        printf("Board successfully created.\n");
                    }
                    else
                    {
                        fprintf(stderr, session.error);
                    }
                }
                break;
                case MENU_OPTION_VIEW:
                case MENU_OPTION_MODIFY:
                case MENU_OPTION_DELETE:
                    session.error = List_getError(session.boards);
                    if (session.error)
                    {
                        // means file cant be open
                        fprintf(stderr, "Operation Failure!\t%s", session.error);
                        break;
                    }

                    if (!session.boards->length)
                    {
                        fprintf(stderr, "No boards created yet.");
                        break;
                    }
                    selectedItemIndex = selectCollectionInterface(session.boards, COLLECTION_TYPE_BOARD); // menu items are started at 1
                    // now selectedItemIndex is the index of board
                    if (selectedItemIndex == MENU_OPTION_GOBACK)
                    {
                        CLEAR_SCREEN();
                        continue; // go back imediately, because this will ignore the Press any key section
                    }
                    selectedItemIndex--; // menu items are started from 1; indeces are from 0; so we decrease the input by 1
                    Board *selectedBoard = (Board *)List_at(session.boards, selectedItemIndex); // remember that ListItem* pointer is also available with list_.lstAccessed ..

                    if (!selectedBoard)
                    {
                        // if index isin valid amd a NULL item returned:
                        printf("You selected: %llu, but the range is: [1-%llu]\n", selectedItemIndex + 1, session.boards->length);
                        fprintf(stderr, "Operation Failure:\n\tYou\'ve selected an out of range item!\n\tNext time, Please select more accurately ...\n");
                        break; // break out of switch
                    }
                    CLEAR_SCREEN(); // clear screen for next section
                    switch (choice)
                    {
                    case MENU_OPTION_VIEW:
                        session.currentBoard = selectedBoard;
                        session.lists = getTaskLists(session.currentBoard);
                        session.error = List_getError(session.lists);
                        if (!session.error)
                        {             // if everything was successfull
                            continue; // continue the loop, so the board will be printed imedately in next if
                        }
                        break; // if there was error, break out of inner switch to print out error
                    case MENU_OPTION_MODIFY:
                    {
                        char oldTitle[MAX_TITLE_LENGTH] = {'\0'};
                        printf("Modify Board Title:\n");
                        printf("\n\tCurrent Title: %s\n", selectedBoard->title);
                        if (!areYouSure("Modifying this board title"))
                            break;
                        PRINT_DASH_ROW();
                        strncpy(oldTitle, selectedBoard->title, MAX_TITLE_LENGTH); // save old title in case something goes wrong!
                        getLine(selectedBoard->title, "\tTitle:\t");
                        if (Boards_save(session.boards, session.user->id)) // if the function returns 1 it means everything successfully worked out.
                        {
                            printf("\nTitle of the board changed successfully.\n");
                        }
                        else
                        {
                            // error happened while saving
                            session.error = List_getError(session.boards);
                            strncpy(selectedBoard->title, oldTitle, MAX_TITLE_LENGTH); // reverse board title, because it hasnt been updated in database
                        }
                    }
                    break;
                    case MENU_OPTION_DELETE:
                        printf("Board that you intend to delete:\n");
                        Board_print(selectedBoard);
                        PRINT_DASH_ROW();
                        if (!areYouSure("Deleting this board?\n**Warning: Everything related to this board will be cleared too, such as the lists on this board, and the Tasks on these lists! "))
                            break;

                        break;
                    }
                    if (session.error)
                    {
                        fprintf(stderr, "%s", session.error);
                    }
                    break; // close the app.
                }
            }
            else if (!session.currentList)
            {
                session.currentBoard->error[0] = '\0'; // reset error value;
                Board_print(session.currentBoard);
                choice = listsMenu(COLLECTION_TYPE_LIST);
                switch (choice)
                {
                case MENU_OPTION_CREATE:
                {
                    TaskList *nextTaskList = createTaskListInterface(session.currentBoard);
                    session.error = TaskList_getError(nextTaskList);
                    if (!session.error) // if board created successfully
                    {
                        List_add(session.lists, nextTaskList);
                        printf("List successfully created and placed on '%s' board.\n", session.currentBoard->title);
                    }
                    else
                    {
                        fprintf(stderr, session.error);
                    }
                }
                break;
                case MENU_OPTION_VIEW:
                case MENU_OPTION_MODIFY:
                case MENU_OPTION_DELETE:
                    session.error = List_getError(session.lists);
                    if (session.error)
                    {
                        // means file cant be open
                        fprintf(stderr, "Operation Failure!\t%s", session.error);
                        break;
                    }
                    if (!session.lists->length)
                    {
                        fprintf(stderr, "No Lists have been added to this board yet.");
                        break;
                    }
                    selectedItemIndex = selectCollectionInterface(session.lists, COLLECTION_TYPE_LIST); // menu items are started at 1
                    if (selectedItemIndex == MENU_OPTION_GOBACK)
                    {
                        CLEAR_SCREEN();
                        continue; // go back imediately, because this will ignore the Press any key section
                    }
                    selectedItemIndex--; // menu items are started from 1; indeces are from 0; so we decrease the input by 1
                    // now selectedItemIndex is the index of list
                    TaskList *selectedTaskList = (TaskList *)List_at(session.lists, selectedItemIndex); // remember that ListItem* pointer is also available with list_.lstAccessed ..

                    if (!selectedTaskList)
                    {
                        // if index isin valid amd a NULL item returned:
                        printf("You selected: %llu, but the range is: [1-%llu]\n", selectedItemIndex + 1, session.lists->length);
                        fprintf(stderr, "Operation Failure:\n\tYou\'ve selected an out of range item!\n\tNext time, Please select more accurately ...\n");
                        break; // break out of switch
                    }

                    CLEAR_SCREEN(); // clear screen for next section
                    switch (choice)
                    {
                    case MENU_OPTION_VIEW:
                        session.currentList = selectedTaskList;
                        session.tasks = getTasks(session.currentList);
                        session.error = List_getError(session.tasks);

                        if (!session.error)
                        {             // if everything was successfull
                            continue; // continue the loop, so the board will be printed imedately in next if
                        }
                        break; // if there was error, break out of inner switch to print out error
                    case MENU_OPTION_MODIFY:
                    {
                        char oldTitle[MAX_TITLE_LENGTH] = {'\0'};
                        printf("Modify List Title:\n");
                        printf("\tCurrent Title: %s\n", selectedTaskList->title);
                        if (!areYouSure("Modifying this list title"))
                            break;
                        PRINT_DASH_ROW();
                        strncpy(oldTitle, selectedTaskList->title, MAX_TITLE_LENGTH); // save old title in case something goes wrong!
                        getLine(selectedTaskList->title, "\tTitle:\t");
                        if (TaskLists_save(session.lists, session.currentBoard->id)) // if the function returns 1 it means everything successfully worked out.
                        {
                            printf("\nTitle of the list changed successfully.\n");
                        }
                        else
                        {
                            // error happened while saving
                            session.error = List_getError(session.lists);
                            strncpy(selectedTaskList->title, oldTitle, MAX_TITLE_LENGTH); // reverse board title, because it hasnt been updated in database
                        }
                    }
                    break;
                    case MENU_OPTION_DELETE:
                        printf("Board that you intend to delete:\n");
                        TaskList_print(selectedTaskList);
                        PRINT_DASH_ROW();
                        if (!areYouSure("Deleting this board?\n**Warning: Everything related to this board will be cleared too, such as the lists on this board, and the Tasks on these lists! "))
                            break;

                        break;
                    }

                    if (session.error)
                    {
                        fprintf(stderr, "%s", session.error);
                    }
                    break; // close the app.
                case MENU_OPTION_GOBACK:
                    free(session.lists);
                    session.currentBoard = NULL;
                    session.currentList = NULL;
                    CLEAR_SCREEN();
                    continue; // immediately go back to boards menu
                }
            }
            else if (!session.currentTask)
            {
                session.currentBoard->error[0] = '\0'; // reset error value;
                session.currentList->error[0] = '\0';  // reset error value;

                TaskList_print(session.currentList);
                printf("\nHow can I help you?\n");
                PRINT_DASH_ROW();
                printf("\t%d\tView Tasks\n\t%d\tAdd New Task\n\t%d\tGo Back",
                       MENU_OPTION_VIEW, MENU_OPTION_CREATE, MENU_OPTION_GOBACK);
                while ((choice = GET_MENU_OPTION()) != MENU_OPTION_CREATE && choice != MENU_OPTION_VIEW && choice != MENU_OPTION_GOBACK)
                    ; // get input until input is valid
                puts("\n");
                switch (choice)
                {
                case MENU_OPTION_CREATE:
                { // because of defining new Variable(Task) this case is wraped in {}, this will clear this local variable at the end of the case
                    Task *nextTask = createTaskInterface(session.currentList);
                    session.error = Task_getError(nextTask);
                    if (!session.error) // if board created successfully
                    {
                        List_add(session.tasks, nextTask);
                        printf("Task successfully added to '%s' List.\n", session.currentList->title);
                    }
                    else
                    {
                        fprintf(stderr, session.error);
                    }
                }
                break;
                case MENU_OPTION_VIEW:
                    session.error = List_getError(session.tasks);
                    if (session.error)
                    {
                        // means file cant be open
                        fprintf(stderr, "Operation Failure!\t%s", session.error);
                        break;
                    }
                    if (!session.tasks->length)
                    {
                        fprintf(stderr, "No Tasks have been added to this list yet.");
                        break;
                    }
                    selectedItemIndex = selectCollectionInterface(session.tasks, COLLECTION_TYPE_TASK); // menu items are started at 1
                    if (selectedItemIndex == MENU_OPTION_GOBACK)
                    {
                        CLEAR_SCREEN();
                        continue;
                    }
                    selectedItemIndex--;// now selectedItemIndex is the index of task
                    Task *selectedTask = (Task *)List_at(session.tasks, selectedItemIndex); // remember that ListItem* pointer is also available with list_.lstAccessed ..

                    if (!selectedTask)
                    {
                        // if index isin valid amd a NULL item returned:
                        printf("You selected: %llu, but the range is: [1-%llu]\n", selectedItemIndex + 1, session.tasks->length);
                        fprintf(stderr, "Operation Failure:\n\tYou\'ve selected an out of range item!\n\tNext time, Please select more accurately ...\n");
                        break; // break out of switch
                    }

                    session.currentTask = selectedTask;
                    CLEAR_SCREEN(); // clear screen for next section
                    continue;       // continue the loop, so the board will be printed imedately in next if
                    // this case doesnt need break; because of using 'continue'
                case MENU_OPTION_GOBACK:
                    free(session.tasks);
                    session.currentList = NULL;
                    CLEAR_SCREEN();
                    continue; // immediately go back to boards menu
                }
            }
            else
            { // board -> list -> task selection completed; now its time to manage a single selected Task
                char taskOption;
                char oldTitle[MAX_TASK_TITLE_LENGTH] = {'\0'};
                Date oldDeadline = session.currentTask->deadline;     // save old deadline,
                Priority oldPriority = session.currentTask->priority; // and save the old priority,
                session.currentBoard->error[0] = '\0';                // reset error value;
                session.currentList->error[0] = '\0';                 // reset error value;
                session.currentTask->error[0] = '\0';                 // reset error value;

                strncpy(oldTitle, session.currentTask->title, MAX_TASK_TITLE_LENGTH); // and save old title in case something goes wrong!
                Task_print(session.currentTask); // print current state of the task
                PRINT_DASH_ROW();
                printf("What do you want to do with this task?\n\t1 Change Title\n\t2 Change Deadline\n\t3 Change Priority\n\t4 Delete\n\t5 Move It To Another List\n\t0 Go Back ");
                // force valid input
                while ((taskOption = GET_MENU_OPTION()) < 0 || taskOption > 4)
                    ;
                PRINT_DASH_ROW();
                switch (taskOption)
                {
                case 1: // change title
                    printf("Change Task Title:\n");
                    printf("\tCurrent Title: %s\n", session.currentTask->title);
                    if (!areYouSure("Changing this Task title"))
                        break;
                    PRINT_DASH_ROW();
                    getLine(session.currentTask->title, "\tTitle:\t");

                    break;

                case 2:
                { // used {} because we define new string below
                    char newDeadline[MAX_DATE_LENGTH] = {'\0'};
                    printf("Change Task Deadline:\n");
                    printf("\tCurrent deadline: %04d-%02d-%02d\n", session.currentTask->deadline.tm_year, session.currentTask->deadline.tm_mon, session.currentTask->deadline.tm_mday);
                    if (!areYouSure("Changing this Task deadline"))
                        break;
                    PRINT_DASH_ROW();
                    getLine(newDeadline, "Date (as Y-M-D): ");
                    if (sscanf(newDeadline, "%d-%d-%d", &(session.currentTask->deadline.tm_year), &(session.currentTask->deadline.tm_mon), &(session.currentTask->deadline.tm_mday)) != 3)
                    {
                        char err[MAX_RESPONSE_LENGTH] = {'\0'};
                        sprintf(err, "Deadline is in wrong format! You must\'ve entered it in this format: Year-Month-Day", MAX_RESPONSE_LENGTH);
                        Task_failure(session.currentTask, err);
                    }
                    break;
                }
                case 3: // change priority
                {
                    char cPriority;
                    printf("Select Priority:\n\t%c: High\n\t%c: Medium\n\t%c: Low", HIGH, MEDIUM, LOW);
                    do
                    {
                        cPriority = GET_CHAR();
                        cPriority = TO_UPPER(cPriority); // convert lower case character to upper
                        session.currentTask->priority = cPriority;
                    } while (session.currentTask->priority != HIGH && session.currentTask->priority != MEDIUM && session.currentTask->priority != LOW); // get priority until a valid value is received!
                    break;
                }
                case 4: // delete task
                    printf("Board that you intend to delete:\n");
                    PRINT_DASH_ROW();
                    if (!areYouSure("Deleting this board?\n**Warning: Everything related to this board will be cleared too, such as the lists on this board, and the Tasks on these lists! "))
                        break;
                    break;
                case 5:
                    //TODO: MOVE TO ANOTHER LIST
                    break;
                case 0:
                    CLEAR_SCREEN();
                    session.currentTask = NULL;
                    continue; // go back imediately; as used previously, using continue will prevent the execution of the Press any key section
                }
                // save changes
                session.error = Task_getError(session.currentTask);

                if (!session.error && Tasks_save(session.tasks, session.currentList->id)) // if the function returns 1 it means everything successfully worked out.
                {
                    printf("\nChanges saved successfully.\n");
                }
                else
                {
                    // error happened while saving
                    session.error = List_getError(session.tasks);
                    fprintf(stderr, "Change was unsuccessful: %s", session.error);
                    // errors on modifying warent shown; CHANGE THAT ALL OVER!!
                    strncpy(session.currentTask->title, oldTitle, MAX_TASK_TITLE_LENGTH); // reverse board title, because it hasnt been updated in database
                    session.currentTask->deadline = oldDeadline;
                    session.currentTask->priority = oldPriority;
                }
            }
        }
        else
        {
            choice = authenticationInterface();
            // if (u)
            //     // if u points to location where an old user object exists
            //     free(u);
            session.user = authenticationAttempt(choice); // choice is matched with MENU_OPTION_LOGIN_ATTEMPT/MENU_OPTION_REGISTERATION_ATTEMPT values
            // so its directly passed to the function
            if (session.user != NULL && session.user->loggedIn)
            {
                printf("Hi %s, Welcome! \n", session.user->name);
                // now load user boards
                // load all boards and list their titles
                session.boards = getBoards(session.user->id);
                session.error = List_getError(session.boards);
                if (session.error)
                {
                    fprintf(stderr, "Fatal Error: %s", session.error);
                    break; // close the app.
                }
            }
            else
            {
                session.error = User_getError(session.user);
                fprintf(stderr, "Authentication Failure: %s", session.error);
            }
        }
        // these two lines below, will run most of the times, except when its needed to use 'continue' to immediately do next loop job
        // such as printing a board/list/task or etc.
        PRESS_KEY_TO_CONTINUE();
        CLEAR_SCREEN(); // clear screen so the terminal doesnt get worked up.
    }
    printf("\nWish to see you soon enough :) Bye!");
}