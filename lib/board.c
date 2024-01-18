#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Board *newBoard() {
    Board *board = (Board *) calloc(sizeof(Board), 1);
    board->id = board->ownerId = 0;
    return board;
}

Board *createBoard(long ownerId, char title[]) {
    Board *board = newBoard();
    time_t now = time(NULL);
    board->ownerId = ownerId;
    // TODO: is it ok to have same name boards?
    strcpy(board->title, title);
    if(now != -1) {
        board->id = (long) now;
        // the boards created by a user will be tored in a file in Boards folder, named by the id of the board owner
        /* LIKE: file:213424234324234
            board1 id
            board1 title
            board2 id
            board2 title
            ...
        */
        SET_DATA_FILE(board->location, FOLDER_BOARDS, ownerId);
        FILE *boardFile = fopen(board->location, "a");
        fprintf("%ld\n%s\n", board->id, board->title); // append new board to file

    } else {
        board->id = 0; // identifier of failure
        board->location[0] = '\0'; // a no location, becqause the board is not saved correctly. 
        fprintf(stderr, "Cannot assign id to this new board!");
    }
    return board;
}