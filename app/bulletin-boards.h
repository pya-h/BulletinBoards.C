#ifndef BULLETIN_BOARDS
#define BULLETIN_BOARDS

    #include <stdio.h>
    #include "../lib/base.h"
    #include "../lib/user.h"
    #include "../lib/board.h"
    #include "../lib/link-list.h"
    #include "../lib/task-list.h"
    #include "../lib/task.h"

    // authentication options
    #define MENU_OPTION_LOGIN_ATTEMPT 1
    #define MENU_OPTION_REGISTERATION_ATTEMPT 2

    // menu options
    // #define MENU_OPTION_VIEW 1
    // #define MENU_OPTION_CREATE 2
    // #define MENU_OPTION_MODIFY 3
    // #define MENU_OPTION_GOBACK 4

    // these two are used in selectCollectionInterface
    #define COLLECTION_TYPE_BOARD 'B'
    #define COLLECTION_TYPE_LIST 'L'
    #define COLLECTION_TYPE_TASK 'T'

    // typedef char MenuOption;
    typedef enum MenuOption {
        MENU_OPTION_VIEW = (char)1, // making menu option to be of type char, so that it doesnt occupy unnecessary space (as int)
        MENU_OPTION_CREATE = (char)2,
        MENU_OPTION_MODIFY = (char)3,
        MENU_OPTION_GOBACK = (char)4
    } MenuOption;

    typedef struct Session {
        User *user;
        Board *currentBoard;
        TaskList *currentList;
        Task *currentTask;

        string error;
        List *boards, *lists, *tasks;
    } Session;
    
    Session newSession();
    void initializeData();
    // authentication menus
    User *authenticationAttempt(short);
    MenuOption authenticationInterface();

    // boards menu and interfaces
    MenuOption boardsMenu();
    Board *createBoardInterface(Long);

    // task lists menu and interfaces
    MenuOption listsMenu(MenuOption);
    TaskList *createTaskListInterface(Board *);
    Task *createTaskInterface(TaskList *);
    // used for selecting board/list
    Long selectCollectionInterface(List *collection, MenuOption collectionType);
#endif //BULLETIN_BOARDS