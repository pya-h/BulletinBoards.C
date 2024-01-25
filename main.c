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
                    selectedItemIndex = selectCollectionInterface(session.boards, COLLECTION_TYPE_BOARD) - 1; // menu items are started at 1
                    // now selectedItemIndex is the index of board
                    if (selectedItemIndex == session.boards->length)
                        break;
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
                        printf("Modify Board Name:\n");
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
                    selectedItemIndex = selectCollectionInterface(session.lists, COLLECTION_TYPE_LIST) - 1; // menu items are started at 1
                    if(selectedItemIndex == session.lists->length)
                        break; // go back selected
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
                        printf("Modify List Name:\n");
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
                TaskList_print(session.currentList);
                choice = listsMenu(COLLECTION_TYPE_TASK);
                switch (choice)
                {
                case MENU_OPTION_CREATE:
                {
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
                case MENU_OPTION_MODIFY:
                case MENU_OPTION_DELETE:
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
                    selectedItemIndex = selectCollectionInterface(session.tasks, COLLECTION_TYPE_TASK) - 1; // menu items are started at 1
                    if(selectedItemIndex == session.tasks->length)
                        break;
                    // now selectedItemIndex is the index of board
                    Task *selectedTask = (Task *)List_at(session.tasks, selectedItemIndex); // remember that ListItem* pointer is also available with list_.lstAccessed ..

                    if (!selectedTask)
                    {
                        // if index isin valid amd a NULL item returned:
                        printf("You selected: %llu, but the range is: [1-%llu]\n", selectedItemIndex + 1, session.tasks->length);
                        fprintf(stderr, "Operation Failure:\n\tYou\'ve selected an out of range item!\n\tNext time, Please select more accurately ...\n");
                        break; // break out of switch
                    }

                    CLEAR_SCREEN(); // clear screen for next section
                    switch (choice)
                    {
                    case MENU_OPTION_VIEW:
                        session.currentTask = selectedTask;                        
                        continue; // continue the loop, so the board will be printed imedately in next if
                    case MENU_OPTION_MODIFY:
                    {
                        char modifyType;
                        Task_print(selectedTask);
                        printf("What field do you want to modify?\n\t1 Title\n\t2 Deadline\n\t3 Priority\n\t4 Cancel");
                        // force valid input
                        while((modifyType = GET_KEY()) != 1 && modifyType != 2 && modifyType != 3 && modifyType != 4);
                        if(modifyType == 4)
                            break;
                        if(modifyType == 1) {
/*
                            char oldTitle[MAX_TITLE_LENGTH] = {'\0'};
                            printf("Modify List Name:\n");
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
                            */
                        }
                    }
                    break;
                    case MENU_OPTION_DELETE:
                        printf("Board that you intend to delete:\n");
                        TaskList_print(selectedTask);
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
                    free(session.tasks);
                    session.currentList = NULL;
                    CLEAR_SCREEN();
                    continue; // immediately go back to boards menu
                }
            }
            else
            {
                Task_print(session.currentTask);
                session.currentTask = NULL;
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

        PRESS_KEY_TO_CONTINUE();
        CLEAR_SCREEN(); // clear screen so the terminal doesnt get worked up.
    }
    printf("\nWish to see you soon enough :) Bye!");
}