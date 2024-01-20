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
short selectBoardMenu(List *);
// THINK: write a professional getch() ?
// TODO: this one will count the number of options and gets key until the max length/count
int main()
{
    User *u = newUser(); // U as for 'you'/username
    short choice;
    prepareDataFolders();

    while (1)
    {
        CLEAR_SCREEN();
        if (u != NULL && u->loggedIn)
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
                    if (!boards) // means file couldnt be open
                        fprintf(stderr, "Operation Failure:\n\tThe database file can not open the boards dataset file.");
                    else
                        choice = selectBoardMenu(boards);
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
            // if (u)
            //     // if u points to location where an old user object exists
            //     free(u);
            u = authenticationAttempt(choice); // choice is matched with MENU_LOGIN_ATTEMPT/MENU_REGISTERATION_ATTEMPT values
            // so its directly passed to the function
            if (u != NULL && !u->loggedIn)
            {
                printf("%s\n", u->error);
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
    string folders[] = {FOLDER_USERS, FOLDER_BOARDS};
    prepareFolder(FOLDER_DATA, 0); // Create the data folder itself
    for (short i = 0; i < numberOfFolders; i++)
    {
        prepareFolder(folders[i], 1); // 1 means create the folder inside data folder
    }
}
// ** Section: Users **
short authenticationMenu()
{
    CLEAR_SCREEN();
    printf("\nHello there! How can I help you?\n");
    PRINT_DASH_ROW();
    printf("\t%d\tLogin\n\t%d\tRegister ", MENU_LOGIN_ATTEMPT, MENU_REGISTERATION_ATTEMPT);
    return GET_KEY();
}

User *authenticationAttempt(short attemptType)
{
    // attemptType: 0 login
    // attemptType: 1 registeration
    printf("\n\n%s:", attemptType == MENU_REGISTERATION_ATTEMPT ? "Registeration" : "Login");
    char username[MAX_USERNAME_LENGTH] = {'\0'}, password[MAX_PASSWORD_LENGTH] = {'\0'};
    getLine(username, "\tUsername:\t");
    getLine(password, "\tPassword:\t");
    if (attemptType == MENU_REGISTERATION_ATTEMPT)
    {
        string validatonResult = validateRegisterationInput(username, password);
        if (validateRegisterationInput)
        {
            // if not null means wrong input:
        }
    }
    return attemptType == MENU_REGISTERATION_ATTEMPT ? registerUser(username, password) : loginUser(username, password);
}

// ** Section: Boards **
short boardsMenu()
{
    printf("\nHow can I help you?\n");
    PRINT_DASH_ROW();
    printf("\t%d\tView Boards\n\t%d\tCreate New Board\n\t%d\tDelete Board ", MENU_VIEW_BOARDS, MENU_CREATE_BOARD, MENU_DELETE_BOARD);
    return GET_KEY();
}

short createNewBoardSection(long ownerId)
{
    char title[MAX_TITLE_LENGTH] = {'\0'};
    printf("\n\nNew Board:");

    getLine(title, "\tTitle:\t");
    Board *board = createBoard(ownerId, title);
    return board->id && board->location; // id and location for failure are set to NULL, so this conditiion will check if there wasnt any error at all.
}

short selectBoardMenu(List *boards)
{
    short choice;
    CLEAR_SCREEN();
    printf("\nWhich board you want to see? [Select & Hit Enter]\n");
    PRINT_DASH_ROW();
    for (int i = 0; i < boards->length; i++)
    {
        Board *board = List_at(boards, i);
        if(!board)
            fprintf(stderr, "\n[X]\t%2d\tUnknown Error!", i + 1);
        if(board->id && board->title) // identiying that the data was read correctly
            printf("\n\t%2d\t%s ", i + 1, board->title);
        else
            fprintf(stderr, "\n[X]\t%2d\t%s", i + 1, board->error);
    }
    scanf("%hd", &choice);
    return choice;
}