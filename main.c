#include "app/bulletin-boards.h"
// THINK: write a professional getch() ?
// TODO: this one will count the number of options and gets key until the max length/count
int main()
{
    MenuOption choice;
    long selectedItemIndex = -1; // choice is of type short and is not suitable with menus with too many items.
    List *boards;
    string error; // used for error handling
    Session session = newSession();
    initializeData();

    while (1)
    {
        if (session.user != NULL && session.user->loggedIn)
        {
            if (!session.board)
            {
                choice = boardsMenu();
                switch (choice)
                {
                case MENU_OPTION_CREATE:
                {
                    Board *newBoard = createNewBoardSection(session.user->id);
                    session.error = Board_getError(newBoard);
                    if (!session.error) // if board created successfully
                    {
                        List_add(boards, newBoard);
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
                    session.error = List_getError(boards);
                    if (session.error)
                    {
                        // means file cant be open
                        fprintf(stderr, "Operation Failure!\t%s", session.error);
                        break;
                    }
                    selectedItemIndex = selectBoardMenu(boards) - 1; // menu items are started at 1
                    // now selectedItemIndex is the index of board
                    Board *selectedBoard = (Board *)List_at(boards, selectedItemIndex); // remember that ListItem* pointer is also available with list_.lstAccessed ..

                    if (!selectedBoard)
                    {
                        // if index isin valid amd a NULL item returned:
                        printf("You selected: %ld, but the range is: [1-%ld]\n", selectedItemIndex + 1, boards->length);
                        fprintf(stderr, "Operation Failure:\n\tYou\'ve selected an out of range item!\n\tNext time, Please select more accurately ...\n");
                        break; // break out of switch
                    }
                    CLEAR_SCREEN();
                    printf("Your selected board is as below:\n\n  Id%6s\t\tOwnerId%4s\t\tTitle\n", " ", " ");
                    session.board = selectedBoard;
                    PRINT_DASH_ROW();
                    printf("%10ld\t\t%10ld\t\t%s\n", selectedBoard->id, selectedBoard->ownerId, selectedBoard->title);
                    continue; // continue the loop, so the console doesnt get clean at the end of the loop
                    // because we want the user to see board data while he sees List menu
                } // this switch doesnt need any default case; because user is forced to select only one of the 3 options provided
            }
            else if(!session.list)
            {
                choice = listsMenu();
                switch (choice)
                {
                case MENU_OPTION_CREATE:
                {
                    TaskList *newTaskList = createNewTaskListSection(session.user->id);
                    session.error = TaskList_getError(newTaskList);
                    if (!session.error) // if board created successfully
                    {
                        List_add(boards, newBoard);
                        // printf("List successfully created and placed on the %s board.\n", session.board->);
                    }
                    else
                    {
                        fprintf(stderr, session.error);
                    }
                }
                break;
                /*case MENU_OPTION_VIEW:
                case MENU_OPTION_MODIFY:
                    session.error = List_getError(boards);
                    if (session.error)
                    {
                        // means file cant be open
                        fprintf(stderr, "Operation Failure!\t%s", session.error);
                        break;
                    }
                    selectedItemIndex = selectBoardMenu(boards) - 1; // menu items are started at 1
                    // now selectedItemIndex is the index of board
                    Board *selectedBoard = (Board *)List_at(boards, selectedItemIndex); // remember that ListItem* pointer is also available with list_.lstAccessed ..

                    if (!selectedBoard)
                    {
                        // if index isin valid amd a NULL item returned:
                        printf("You selected: %ld, but the range is: [1-%ld]\n", selectedItemIndex + 1, boards->length);
                        fprintf(stderr, "Operation Failure:\n\tYou\'ve selected an out of range item!\n\tNext time, Please select more accurately ...\n");
                        break; // break out of switch
                    }
                    CLEAR_SCREEN();
                    printf("Your selected board is as below:\n\n  Id%6s\t\tOwnerId%4s\t\tTitle\n", " ", " ");
                    session.board = selectedBoard;
                    PRINT_DASH_ROW();
                    printf("%10ld\t\t%10ld\t\t%s\n", selectedBoard->id, selectedBoard->ownerId, selectedBoard->title);
                    continue; // continue the loop, so the console doesnt get clean at the end of the loop
                    // because we want the user to see board data while he sees List menu
                */
                }
            }
        }
        else
        {
            choice = authenticationMenu();
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
                boards = getBoards(session.user->id);
                session.error = List_getError(boards);
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