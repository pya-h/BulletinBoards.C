#ifndef BOARD_H
#define BOARD_H
#include "base.h"

#define FOLDER_BOARDS "Boards"
#define MAX_BOARD_FILE_ROW_LENGTH MAX_TITLE_LENGTH + 20 // 20 as the length of id and some extra characters such as comma(delimiter) or double quatation
typedef struct Board
{
    long id; // will be set by the current time value
    // id will be used for ownership of other items (such as tasks)
    char title[MAX_TITLE_LENGTH];
    long ownerId; // the id of the user tat has created this board
    // place where this board data is stored
    char location[MAX_FILENAME_LENGTH];

    char error[MAX_RESPONSE_LENGTH];
} Board;

// prototypes:
Board *newBoard();
Board *createBoard(long ownerId, char title[]);
List *getBoards(long ownerId);
short saveBoards(List *boards);

void resetBoard(Board *board);
void throwBoardError(Board *board, string msg);
#endif // BOARD_H