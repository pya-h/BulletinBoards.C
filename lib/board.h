#ifndef BOARD_H
#define BOARD_H
#include "base.h"

#define FOLDER_BOARDS "Boards"
#define MAX_BOARD_NAME_LENGTH 64

struct Board {
    long id; // will be set by the current time value
    // id will be used for ownership of other items (such as tasks)
    char name[MAX_BOARD_NAME_LENGTH];

    // place where this board data is stored
    char location[MAX_FILENAME_LENGTH];

};

typedef struct Board Board;

// prototypes:
Board *newBoard();
Board *createBoard(long ownerId, char boardName[]);

#endif // BOARD_H