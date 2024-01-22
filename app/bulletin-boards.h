#ifndef BULLETIN_BOARDS
#define BULLETIN_BOARDS

    #include <stdio.h>
    #include "../lib/base.h"
    #include "../lib/user.h"
    #include "../lib/board.h"
    #include "../lib/link-list.h"
    #include "../lib/task-list.h"

    // authentication options
    #define MENU_OPTION_LOGIN_ATTEMPT 1
    #define MENU_OPTION_REGISTERATION_ATTEMPT 2

    // boards options
    #define MENU_OPTION_VIEW 1
    #define MENU_OPTION_CREATE 2
    #define MENU_OPTION_MODIFY 3


    typedef char MenuOption;

    typedef struct Session {
        User *user;
        Board *board;
        TaskList *list;
        string error;
        List *boards, *lists;
    } Session;
    
    Session newSession();
    void initializeData();
    User *authenticationAttempt(short);
    MenuOption authenticationInterface();
    MenuOption boardsMenu();
    Board *createBoardInterface(long);
    long selectBoardInterface(List *);
    MenuOption listsMenu();
    TaskList *createTaskListInterface(Board *containerBoard);
#endif //BULLETIN_BOARDS