#include <stdio.h>
#include "lib/base.h"
#include "lib/user.h"
#include "lib/board.h"

// authentication options
#define MENU_LOGIN_ATTEMPT 1
#define MENU_REGISTERATION_ATTEMPT 2

// boards options
#define MENU_VIEW_BOARDS 1
#define MENU_CREATE_BOARD 2
#define MENU_DELETE_BOARD 3

void prepareDataFolders();
User *authenticationAttempt(short);
short authenticationMenu();
short boardsMenu();
short createNewBoardSection(long);

int main()
{
    User *u = newUser(); // U as for 'you'/username
    short choice;
    prepareDataFolders();

    while (1)
    {
        CLEAR_SCREEN();
        if (u->loggedIn)
        {
            if (!u->currentBoardId)
            {
                List *boards;
                choice = boardsMenu();
                switch (choice)
                {
                case MENU_CREATE_BOARD:
                    if (createNewBoardSection(u->id))
                    {
                        printf("Board successfully created.\n");
                    }
                    else
                    {
                        printf("Something wen\'t wrong while creating your board; Please Try again later.");
                    }
                    break;
                case MENU_VIEW_BOARDS:
                    boards = getBoards(u->id);
                    printf("%d", boards->length);
                    for (int i = 0; i < boards->length; i++)
                    {
                        Board *b = List_at(boards, i);
                        printf("\n%ld\t%s", b->id, b->title);
                    }
                    break;
                }
            }
            else
            {
                // TODO:
                // if user has selected a board
            }
        }
        else
        {
            choice = authenticationMenu();
            u = authenticationAttempt(choice); // choice is matched with MENU_LOGIN_ATTEMPT/MENU_REGISTERATION_ATTEMPT values
            // so its directly passed to the function
            if (!u->loggedIn)
            {
                printf("Username or password is wrong! Try again ...\n");
            }
            else
            {
                printf("Hi %s, Welcome! \n", u->name);
            }
        }
        PRESS_KEY_TO_CONTINUE();
    }
}

void prepareDataFolders()
{
    // will make sure all folders are created & ready to hold info.
    const short numberOfFolders = 2;
    char *folders[] = {FOLDER_USERS, FOLDER_BOARDS};
    prepareFolder(FOLDER_DATA, 0); // Create the data folder itself
    for (short i = 0; i < numberOfFolders; i++)
    {
        prepareFolder(folders[i], 1); // 1 means create the folder inside data folder
    }
}
short authenticationMenu()
{
    CLEAR_SCREEN();
    printf("\nHello there! How can I help you?\n");
    PRINT_DASH_ROW();
    printf("\t%d Login\n\t%d Register ", MENU_LOGIN_ATTEMPT, MENU_REGISTERATION_ATTEMPT);
    return GET_KEY();
}

User *authenticationAttempt(short attemptType)
{
    // attemptType: 0 login
    // attemptType: 1 registeration
    printf("\n%s:\n", attemptType == MENU_REGISTERATION_ATTEMPT ? "Registeration" : "Login");
    char username[MAX_USERNAME_LENGTH], password[MAX_PASSWORD_LENGTH];
    printf("\tUsername: ");
    gets(username);
    printf("\tPassword: ");
    gets(password);
    if (attemptType == MENU_REGISTERATION_ATTEMPT)
    {
        char *validatonResult = validateRegisterationInput(username, password);
        if (validateRegisterationInput)
        {
            // if not null means wrong input:
        }
    }
    return attemptType == MENU_REGISTERATION_ATTEMPT ? registerUser(username, password) : loginUser(username, password);
}

short boardsMenu()
{
    printf("\nHow can I help you?\n");
    PRINT_DASH_ROW();
    printf("\t%d View Boards\n\t%d Create New Board\n\t%d Delete Board ", MENU_VIEW_BOARDS, MENU_CREATE_BOARD, MENU_DELETE_BOARD);
    return GET_KEY();
}

short createNewBoardSection(long ownerId)
{
    char title[MAX_TITLE_LENGTH] = {'\0'};
    printf("\nNew Board:\n\tTitle: ");
    gets(title);
    Board *board = createBoard(ownerId, title);
    return board->id && board->location; // id and location for failure are set to NULL, so this conditiion will check if there wasnt any error at all.
}