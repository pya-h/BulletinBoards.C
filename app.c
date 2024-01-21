#include <stdio.h>
#include "lib/base.h"
#include "lib/user.h"
#include "lib/board.h"
#include "lib/link-list.h"

#include <string.h>

// authentication options
#define MENU_OPTION_LOGIN_ATTEMPT 1
#define MENU_OPTION_REGISTERATION_ATTEMPT 2

// boards options
#define MENU_OPTION_VIEW_BOARDS 1
#define MENU_OPTION_CREATE_BOARD 2
#define MENU_OPTION_MODIFY_BOARD 3

typedef char MenuOption;

void initializeData();
User *authenticationAttempt(short);
MenuOption authenticationMenu();
MenuOption boardsMenu();
Board *createNewBoardSection(long);
long selectBoardMenu(List *);
// THINK: write a professional getch() ?
// TODO: this one will count the number of options and gets key until the max length/count
int main()
{
    User *u = newUser(); // U as for 'you'/username
    MenuOption choice;
    long selectedItemIndex = -1; // choice is of type short and is not suitable with menus with too many items.
    List *boards;
    string error; // used for error handling

    initializeData();

    PRESS_KEY_TO_CONTINUE();
    while (1)
    {
        CLEAR_SCREEN();
        if (u != NULL && u->loggedIn)
        {
            if (!u->currentBoardId)
            {
                choice = boardsMenu();
                switch (choice)
                {
                case MENU_OPTION_CREATE_BOARD:
                    {
                        Board *newBoard = createNewBoardSection(u->id);
                        error = Board_getError(newBoard);
                        if (!error) // if board created successfully
                        {
                            List_add(boards, newBoard);
                            printf("Board successfully created.\n");
                        }
                        else
                        {
                            fprintf(stderr, error);
                        }
                    }
                    break;
                case MENU_OPTION_VIEW_BOARDS:
                case MENU_OPTION_MODIFY_BOARD:
                    error = List_getError(boards);
                    if (error)
                    {
                        // means file cant be open
                        fprintf(stderr, "Operation Failure!\t%s", error);
                        break;
                    }
                    selectedItemIndex = selectBoardMenu(boards) - 1; // menu items are started at 1
                    // now selectedItemIndex is the index of board
                    Board *selectedBoard = (Board *)List_at(boards, selectedItemIndex); // remember that ListItem* pointer is also available with list_.lstAccessed ..

                    if (!selectedBoard)
                    {
                        // if index isin valid amd a NULL item returned:
                        printf("You selected: %ld, but the range is: [1-%ld]\n", selectedItemIndex + 1,boards->length);
                        fprintf(stderr, "Operation Failure:\n\tYou\'ve selected an out of range item!\n\tNext time, Please select more accurately ...\n");
                        break; // break out of switch
                    }
                    CLEAR_SCREEN();
                    printf("Your selected board is as below:\n\n  Id%6s\t\tOwnerId%4s\t\tTitle\n", " ", " ");
                    PRINT_DASH_ROW();
                    printf("%10ld\t\t%10ld\t\t%s\n", selectedBoard->id, selectedBoard->ownerId, selectedBoard->title);
                }
            }
        }
        else
        {
            choice = authenticationMenu();
            // if (u)
            //     // if u points to location where an old user object exists
            //     free(u);
            u = authenticationAttempt(choice); // choice is matched with MENU_OPTION_LOGIN_ATTEMPT/MENU_OPTION_REGISTERATION_ATTEMPT values
            // so its directly passed to the function
            if (u != NULL && u->loggedIn)
            {
                printf("Hi %s, Welcome! \n", u->name);
                // now load user boards
                // load all boards and list their titles
                boards = getBoards(u->id);
                error = List_getError(boards);
                if(error)
                {
                    fprintf(stderr, "Fatal Error: %s", error);
                    break; // close the app.
                }
            }
            else
            {
                error = User_getError(u);
                fprintf(stderr, "Authentication Failure: %s", error);
            }
        }

        PRESS_KEY_TO_CONTINUE();
    }
    printf("\nWish to see you soon enough :) Bye!");
}

void initializeData()
{
    // will make sure all folders are created & ready to hold info.
    const short numberOfFolders = 2;
    string folders[] = {FOLDER_USERS, FOLDER_BOARDS};
    prepareFolder(FOLDER_DATA, 0); // Create the data folder itself
    for (short i = 0; i < numberOfFolders; i++)
    {
        prepareFolder(folders[i], 1); // 1 means create the folder inside data folder
    }
}
// ** Section: Users **
MenuOption authenticationMenu()
{
    MenuOption option;
    CLEAR_SCREEN();
    printf("\nHello there! How can I help you?\n");
    PRINT_DASH_ROW();
    printf("\t%d\tLogin\n\t%d\tRegister ", MENU_OPTION_LOGIN_ATTEMPT, MENU_OPTION_REGISTERATION_ATTEMPT);
    while ((option = GET_KEY()) != MENU_OPTION_LOGIN_ATTEMPT && option != MENU_OPTION_REGISTERATION_ATTEMPT)
        ; // get input until the input is valid
    return option;
}

User *authenticationAttempt(short attemptType)
{
    // attemptType: 0 login
    // attemptType: 1 registeration
    printf("\n\n%s:", attemptType == MENU_OPTION_REGISTERATION_ATTEMPT ? "Registeration" : "Login");
    char username[MAX_ANY_STRING_LENGTH] = {'\0'}, password[MAX_ANY_STRING_LENGTH] = {'\0'};
    getLine(username, "\tUsername:\t");
    getLine(password, "\tPassword:\t");

    return attemptType == MENU_OPTION_REGISTERATION_ATTEMPT ? registerUser(username, password) : loginUser(username, password);
}

// ** Section: Boards **
MenuOption boardsMenu()
{
    MenuOption option;
    printf("\nHow can I help you?\n");
    PRINT_DASH_ROW();
    printf("\t%d\tView/Open Boards\n\t%d\tCreate New Board\n\t%d\tDelete/Edit Board ",
           MENU_OPTION_VIEW_BOARDS, MENU_OPTION_CREATE_BOARD, MENU_OPTION_MODIFY_BOARD);
    while ((option = GET_KEY()) != MENU_OPTION_CREATE_BOARD &&
           option != MENU_OPTION_MODIFY_BOARD && option != MENU_OPTION_VIEW_BOARDS)
        return option;
}

Board *createNewBoardSection(long ownerId)
{
    char title[MAX_ANY_STRING_LENGTH];
    printf("\n\nNew Board:");

    getLine(title, "\tTitle:\t");
    return createBoard(ownerId, title);
}

long selectBoardMenu(List *boards)
{
    long choice;
    string error;
    CLEAR_SCREEN();
    printf("\nWhich board you want to see? [Select & Hit Enter]\n");
    PRINT_DASH_ROW();
    for (int i = 0; i < boards->length; i++)
    {
        Board *board = List_at(boards, i);
        error = Board_getError(board);
        if (error)
            fprintf(stderr, "\nX\t%2d\t%s", i + 1, error);
        else
            printf("\n\t%2d\t%s ", i + 1, board->title);

    }
    scanf("%ld", &choice);
    return choice;
}