#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void resetBoard(Board *board)
{
    board->location[0] = '\0';      // a no location, becqause the board is not saved.
    board->id = board->ownerId = 0; // identifier of failure
}

Board *newBoard()
{
    Board *board = (Board *)calloc(1, sizeof(Board));
    board->location[0] = '\0';
    board->id = board->ownerId = 0;
    return board;
}

Board *createBoard(long ownerId, char title[])
{
    Board *board = newBoard();
    time_t now = time(NULL);
    board->ownerId = ownerId;
    // TODO: is it ok to have same name boards?
    strcpy(board->title, title);
    if (now != -1)
    {
        board->id = (long)now;
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
        if (boardFile)
        {
            fprintf(boardFile, "%ld\n%s\n", board->id, board->title); // append new board to file
        }
        else
        {
            fprintf(stderr, "Cannot save board data!");
            resetBoard(board); // return an empty board again, as an identification of error!
        }
        fclose(boardFile);
    }
    else
    {
        resetBoard(board);         // identifier of failure
        board->location[0] = '\0'; // a no location, becqause the board is not saved correctly.
        fprintf(stderr, "Cannot assign id to this new board!");
    }
    return board;
}

List *getBoards(long ownerId)
{
    // read all the boards from the file
    List *boards = newList();
    char boardFilename[MAX_FILENAME_LENGTH] = {'\0'};
    SET_DATA_FILE(boardFilename, FOLDER_BOARDS, ownerId); // now boardFile contains the address of the board file that contains desired user board list ata.
    FILE *boardFile = fopen(boardFilename, "r");
    if (boardFile)
    {

        for (Board *nextBoard = newBoard(); !feof(boardFile); nextBoard = newBoard())
        {
            // each board occupies to lines
            // first line is its id and the second is the title
            fscanf(boardFile, "%ld\n", &nextBoard->id);
            if (!nextBoard->id)
                break;
            // the title of the board may contain spaces; using fscanf will ignore what comes after the space!
            fgets(nextBoard->title, MAX_TITLE_LENGTH, boardFile);
            // fgets returns current line of the file, containing ['\n'] next line character;
            removeNextlineCharacter(nextBoard->title);

            // nextBoard on each step of the loop, will be used to read board data;
            // then the memory it points to will be added as the boards list item,
            // then nextBoard will occupy a new place on memory
            List_add(boards, nextBoard);
        }
    }
    else
    {
        fprintf(stderr, "Cannot open boards file!");
    }
    fclose(boardFile);
    return boards;
}

short saveBoards(List *boards)
{
    // this is used when a board is modified or deleted
    // then the app should remove boards file data and replace its data with the updated data
    // that is stored in boards list
}