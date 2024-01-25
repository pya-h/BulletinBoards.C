
#include "bulletin-boards.h"

Session newSession()
{
    Session s;
    s.user = newUser();
    s.currentBoard = NULL;
    s.currentList = NULL;
    s.currentTask = NULL;
    return s;
}

void initializeData()
{
    // will make sure all folders are created & ready to hold info.
    const short numberOfFolders = 4;
    string folders[] = {FOLDER_USERS, FOLDER_BOARDS, FOLDER_LISTS, FOLDER_TASKS};
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
    PRINT_DASH_ROW();
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
    printf("\t%d\tView/Open Boards\n\t%d\tCreate New Board\n\t%d\tModify Board Title\n\t%d\tDelete Board Title ",
           MENU_OPTION_VIEW, MENU_OPTION_CREATE, MENU_OPTION_MODIFY, MENU_OPTION_DELETE);
    while ((option = GET_KEY()) != MENU_OPTION_CREATE && option != MENU_OPTION_DELETE &&
           option != MENU_OPTION_MODIFY && option != MENU_OPTION_VIEW) ; //get input until its valid

    puts("\n");
    return option;
}

Board *createBoardInterface(Long ownerId)
{
    char title[MAX_ANY_STRING_LENGTH];
    printf("\n\nBoard Creation:");
    PRINT_DASH_ROW();
    getLine(title, "\tTitle:\t");
    return createBoard(ownerId, title);
}

TaskList *createTaskListInterface(Board *containerBoard)
{
    char title[MAX_ANY_STRING_LENGTH];
    printf("\n\nCreate List:");
    PRINT_DASH_ROW();
    getLine(title, "\tTitle:\t");
    return createTaskList(containerBoard, title);
}
// ** Section: Boards **
MenuOption listsMenu(MenuOption collectioType)
{
    MenuOption option;
    printf("\nHow can I help you?\n");
    PRINT_DASH_ROW();
    string collectionName = collectioType == COLLECTION_TYPE_LIST ? "List" : "Task";
    printf("\t%d\tView/Open %ss\n\t%d\tCreate New %s\n\t%d\tModify %s\n\t%d\tDelete %s\n\t%d\tGo Back",
           MENU_OPTION_VIEW, collectionName, MENU_OPTION_CREATE, 
           collectionName, MENU_OPTION_MODIFY, collectionName, MENU_OPTION_DELETE, collectionName, MENU_OPTION_GOBACK);
    while ((option = GET_KEY()) != MENU_OPTION_CREATE && option != MENU_OPTION_DELETE &&
           option != MENU_OPTION_MODIFY && option != MENU_OPTION_VIEW && option != MENU_OPTION_GOBACK) ; // get input until input is valid
    
    puts("\n");
    
    return option;
}

// TODO: THIS FUNCTION and selectBoardInterface may be merged
Long selectCollectionInterface(List *collection, MenuOption collectionType)
{
    Long choice;
    string title;
    string error;
    CLEAR_SCREEN();
    printf("\nWhich %s you want to see? [Select & Hit Enter]\n", collectionType == COLLECTION_TYPE_BOARD ? "Board" :
        collectionType == COLLECTION_TYPE_LIST ? "List" : "Task");
    PRINT_DASH_ROW();
    for (Long i = 0; i < collection->length; i++)
    {
        if (collectionType == COLLECTION_TYPE_BOARD)
        {
            Board *board = (Board*)List_at(collection, i);
            error = Board_getError(board);
            title = board->title;
        }
        else if(collectionType == COLLECTION_TYPE_LIST)
        {
            TaskList *taskList = (TaskList*)List_at(collection, i);
            error = TaskList_getError(taskList);
            title = taskList->title;
        } else {
            // task
            Task *task = (Task*)List_at(collection, i);
            error = Task_getError(task);
            title = task->title;
        }

        if (error)
            fprintf(stderr, "\nX\t%2d\t%s", i + 1, error);
        else
            printf("\n%10llu\t%s ", i + 1, title);
    }
    printf("\n%10llu\tGo Back ", collection->length + 1);
    scanf("%llu", &choice);
    return choice;
}

Task *createTaskInterface(TaskList *containerTaskList) {
    char title[MAX_ANY_STRING_LENGTH];
    char deadline[MAX_DATE_LENGTH];
    printf("\n\nAdd Task:");
    PRINT_DASH_ROW();
    getLine(title, "\tTitle:\t");
    printf("Select Priority:\n\t%c: High\n\t%c: Medium\n\t%c: Low", HIGH, MEDIUM, LOW);
    Priority priority;
    do {
        char c = GET_CHAR();
        c = TO_UPPER(c); // convert lower case character to upper
        priority = c;
    } while(priority != HIGH && priority != MEDIUM && priority != LOW); // get priority until a valid value is received!
    getLine(deadline, "Date (as Y-M-D): ");
    return createTask(containerTaskList, title, priority, deadline);
}

short areYouSure(string message)
{
    printf("Are you sure on %s? [Y]ES, [N]O ", message);
    char answer;
    do {
        answer = GET_CHAR();
        answer = TO_UPPER(answer); // convert lower case character to upper
    } while(answer != 'Y' && answer != 'N');
    puts("\n");
    return answer == 'Y';
}