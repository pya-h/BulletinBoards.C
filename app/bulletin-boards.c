
#include "bulletin-boards.h"

Session newSession()
{
    Session s;
    s.user = newUser();
    s.board = NULL;
    s.list = NULL;
    return s;
}

void initializeData()
{
    // will make sure all folders are created & ready to hold info.
    const short numberOfFolders = 3;
    string folders[] = {FOLDER_USERS, FOLDER_BOARDS, FOLDER_LISTS};
    prepareFolder(FOLDER_DATA, 0); // Create the data folder itself
    for (short i = 0; i < numberOfFolders; i++)
    {
        prepareFolder(folders[i], 1); // 1 means create the folder inside data folder
    }
}
// ** Section: Users **
MenuOption authenticationInterface()
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
        ;
    return option;
}

Board *createBoardInterface(long ownerId)
{
    char title[MAX_ANY_STRING_LENGTH];
    printf("\n\nBoard Creation:");

    getLine(title, "\tTitle:\t");
    return createBoard(ownerId, title);
}

long selectBoardInterface(List *boards)
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

TaskList *createTaskListInterface(Board *containerBoard)
{
    char title[MAX_ANY_STRING_LENGTH];
    printf("\n\nList Creation");

    getLine(title, "\tTitle:\t");
    return createTaskList(containerBoard, title);
}
// ** Section: Boards **
MenuOption listsMenu()
{
    MenuOption option;
    printf("\nHow can I help you?\n");
    PRINT_DASH_ROW();
    printf("\t%d\tView/Open Lists\n\t%d\tCreate New List\n\t%d\tDelete/Edit List ",
           MENU_OPTION_VIEW, MENU_OPTION_CREATE, MENU_OPTION_MODIFY);
    while ((option = GET_KEY()) != MENU_OPTION_CREATE &&
           option != MENU_OPTION_MODIFY && option != MENU_OPTION_VIEW)
        ;
    return option;
}