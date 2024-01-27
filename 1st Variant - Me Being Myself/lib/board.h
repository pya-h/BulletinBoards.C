#ifndef BOARD_H
#define BOARD_H
    #include "base.h"
    #include "link-list.h"
    #define FOLDER_BOARDS "Boards"
    #define MAX_BOARD_FILE_ROW_LENGTH MAX_TITLE_LENGTH + MAX_LONG_NUMBER_LENGTH + 10 // 30 as the length of id and some extra characters such as comma(delimiter) or double quatation
    typedef struct Board
    {
        Long id; // will be set by the current time value
        // id will be used for ownership of other items (such as tasks)
        char title[MAX_TITLE_LENGTH], 
            error[MAX_RESPONSE_LENGTH];
        Long ownerId; // the id of the user tat has created this board
    } Board;

    // prototypes:
    Board *newBoard();
    Board *createBoard(Long, char []);
    List *getBoards(Long);
    short Boards_save(List *, Long);
    short Board_delete(Board *);
    string Board_getError(Board *);
    void Board_reset(Board *);
    void Board_failure(Board *, string);
    void Board_print(Board *);
#endif // BOARD_H