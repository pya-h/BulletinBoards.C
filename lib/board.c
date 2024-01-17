#include "board.h"
#include <stdio.h>
#include <stdlib.h>

Board *newBoard() {
    Board *board = (Board *) calloc(sizeof(Board), 1);
    return board;
}

Board *createBoard(long ownerId, char boardName[]) {
    Board *board = newBoard();
    
}