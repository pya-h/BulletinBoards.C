#ifndef BOARD_H
#define BOARD_H
#include "base.h"

#define FOLDER_BOARDS "Boards"

typedef struct Board
{
    long id; // will be set by the current time value
    // id will be used for ownership of other items (such as tasks)
    char title[MAX_TITLE_LENGTH];
    long ownerId; // the id of the user tat has created this board
    // place where this board data is stored
    char location[MAX_FILENAME_LENGTH];
} Board;

// prototypes:
Board *newBoard();
Board *createBoard(long ownerId, char title[]);
List *getBoards(long ownerId);

void resetBoard(Board *board);
#endif // BOARD_H