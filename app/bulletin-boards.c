
#include "bulletin-boards.h"

Session newSession() {
    Session s;
    s.user = newUser();
    s.board = NULL;
    s.list = NULL;
    return s;
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
           MENU_OPTION_VIEW, MENU_OPTION_CREATE, MENU_OPTION_MODIFY);
    while ((option = GET_KEY()) != MENU_OPTION_CREATE &&
           option != MENU_OPTION_MODIFY && option != MENU_OPTION_VIEW)
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