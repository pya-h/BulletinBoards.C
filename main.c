#include "app/bulletin-boards.h"
#include "lib/task.h"
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
                    session.error = List_getError(session.boards);
                    if (session.error)
                    {
                        // means file cant be open
                        fprintf(stderr, "Operation Failure!\t%s", session.error);
                        break;
                    }
                    selectedItemIndex = selectCollectionInterface(session.boards, COLLECTION_TYPE_BOARD) - 1; // menu items are started at 1
                    // now selectedItemIndex is the index of board
                    Board *selectedBoard = (Board *)List_at(session.boards, selectedItemIndex); // remember that ListItem* pointer is also available with list_.lstAccessed ..

                    if (!selectedBoard)
                    {
                        // if index isin valid amd a NULL item returned:
                        printf("You selected: %llu, but the range is: [1-%llu]\n", selectedItemIndex + 1, session.boards->length);
                        fprintf(stderr, "Operation Failure:\n\tYou\'ve selected an out of range item!\n\tNext time, Please select more accurately ...\n");
                        break; // break out of switch
                    }
                    session.currentBoard = selectedBoard;
                    session.lists = getTaskLists(session.currentBoard);
                    session.error = List_getError(session.lists);
                    if (session.error)
                    {
                        fprintf(stderr, "Fatal Error: %s", session.error);
                        break; // close the app.
                    }
                    CLEAR_SCREEN();
                    continue; // continue the loop, so the board will be printed imedately in next if
                }             // this switch doesnt need any default case; because user is forced to select only one of the 3 options provided
            }
            else if (!session.currentList)
            {
                Board_print(session.currentBoard);
                choice = listsMenu();
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
                    session.error = List_getError(session.lists);
                    if (session.error)
                    {
                        // means file cant be open
                        fprintf(stderr, "Operation Failure!\t%s", session.error);
                        break;
                    }
                    selectedItemIndex = selectCollectionInterface(session.lists, COLLECTION_TYPE_BOARD) - 1; // menu items are started at 1
                    // now selectedItemIndex is the index of board
                    TaskList *selectedTaskList = (TaskList *)List_at(session.lists, selectedItemIndex); // remember that ListItem* pointer is also available with list_.lstAccessed ..

                    if (!selectedTaskList)
                    {
                        // if index isin valid amd a NULL item returned:
                        printf("You selected: %llu, but the range is: [1-%llu]\n", selectedItemIndex + 1, session.lists->length);
                        fprintf(stderr, "Operation Failure:\n\tYou\'ve selected an out of range item!\n\tNext time, Please select more accurately ...\n");
                        break; // break out of switch
                    }
                    session.currentList = selectedTaskList;
                    // TODO: Load TASKS here
                    // then check error
                    /*
                    session.tasks = getTasks(session.currentList->id);
                    session.error = List_getError(session.lists);
                    if (session.error)
                    {
                        fprintf(stderr, "Fatal Error: %s", session.error);
                        break; // close the app.
                    }*/
                    CLEAR_SCREEN();
                    continue; // continue the loop, so the board will be printed imedately in next if
                    // because we want the user to see board data while he sees List menu
                case MENU_OPTION_GOBACK:
                    free(session.lists);
                    session.currentBoard = NULL;
                    session.currentList = NULL;
                    CLEAR_SCREEN();
                    continue; // immediately go back to boards menu
                }
            }
            else
            {
                TaskList_print(session.currentList);
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