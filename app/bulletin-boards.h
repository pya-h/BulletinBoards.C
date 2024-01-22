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
    #define MENU_OPTION_GOBACK 4

    // these two are used in selectCollectionInterface
    #define COLLECTION_TYPE_BOARD 'B'
    #define COLLECTION_TYPE_LIST 'L'

    typedef char MenuOption;

    typedef struct Session {
        User *user;
        Board *currentBoard;
        TaskList *currentList;
        string error;
        List *boards, *lists;
    } Session;
    
    Session newSession();
    void initializeData();
    // authentication menus
    User *authenticationAttempt(short);
    MenuOption authenticationInterface();

    // boards menu and interfaces
    MenuOption boardsMenu();
    Board *createBoardInterface(long);

    // task lists menu and interfaces
    MenuOption listsMenu();
    TaskList *createTaskListInterface(Board *);

    // used for selecting board/list
    long selectCollectionInterface(List *collection, MenuOption collectionType);
#endif //BULLETIN_BOARDS