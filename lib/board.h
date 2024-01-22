#ifndef BOARD_H
#define BOARD_H
    #include "base.h"
    #include "link-list.h"
    #define FOLDER_BOARDS "Boards"
    #define MAX_BOARD_FILE_ROW_LENGTH MAX_TITLE_LENGTH + 20 // 20 as the length of id and some extra characters such as comma(delimiter) or double quatation
    typedef struct Board
    {
        long id; // will be set by the current time value
        // id will be used for ownership of other items (such as tasks)
        char title[MAX_TITLE_LENGTH];
        long ownerId; // the id of the user tat has created this board
        char error[MAX_RESPONSE_LENGTH];
    } Board;

    // prototypes:
    Board *newBoard();
    Board *createBoard(long ownerId, char title[]);
    List *getBoards(long ownerId);
    short Boards_save(List *boards);
    string Board_getError(Board *);
    void Board_reset(Board *board);
    void Board_failure(Board *board, string msg);
    void Board_print(Board *);
#endif // BOARD_H