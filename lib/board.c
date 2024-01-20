#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void resetBoard(Board *board)
{
    board->location[0] = board->error[0] = '\0';    // a no location, becqause the board is not saved.
    board->id = board->ownerId = 0; // identifier of failure
}

Board *newBoard()
{
    Board *board = (Board *)calloc(1, sizeof(Board));
    resetBoard(board);
    return board;
}

void throwBoardError(Board *board, string msg)
{
    resetBoard(board);
    sprintf(board->error, "Unexpected Behaviour:\t%s\n", msg);
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

        SET_DATA_FILE(board->location, FOLDER_BOARDS, ownerId);
        if (!fileExists(board->location))
        {
            // creste the file and add the header row
            FILE *boardFile = fopen(board->location, "w");
            fprintf(boardFile, "Id%sBoard Title\n", COLUMN_DELIMITER);
            fclose(boardFile);
        }
        // now add new items
        FILE *boardFile = fopen(board->location, "a");
        if (boardFile)
        {
            fprintf(boardFile, "%ld%s\"%s\"\n", board->id, COLUMN_DELIMITER, board->title); // append new board to file
        }
        else
        {
            throwBoardError(board, "Cannot save board data!");
        }
        fclose(boardFile);
    }
    else
    {
        throwBoardError(board, "Cannot assign id to this new board!");
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
    if (!boardFile)
    {
        fclose(boardFile);
        return NULL; // when this function returns NULL, it means there was an error opening the boards database.
    }
    Board *nextBoard = newBoard(); // this is used to read each board data
    string row = String(MAX_BOARD_FILE_ROW_LENGTH);
    if (!feof(boardFile) && fgets(row, MAX_BOARD_FILE_ROW_LENGTH, boardFile) != NULL)
    { // the first row are headers(titles); also its a good practice to check that as an insurance that file data is stored correctly.
        for (; !feof(boardFile) && fgets(row, MAX_BOARD_FILE_ROW_LENGTH, boardFile) != NULL; nextBoard = newBoard())
        {

            removeNextlineCharacter(row);
            // the second call to fgets will read the credential line
            const string id = (string)strtok(row, COLUMN_DELIMITER); // split the text by COLUMN_DELIMITER[,] character
            string title = (string)strtok(NULL, COLUMN_DELIMITER);   // get the next column
            title = trimColumnValue(title);
            if (!id || !title)
            {
                if (!feof(boardFile)) // if file is not ended and this condition happended, then the data of this board is corrupted
                    throwBoardError(nextBoard, "It seems the data related to this board is corrupted!");
                continue;
            }

            // each board occupies to lines
            // first line is its id and the second is the title
            strcpy(nextBoard->title, title);
            nextBoard->id = atol(id); // convert read id to long
            if (!nextBoard->id)
            {
                throwBoardError(nextBoard, "Could not read the id property of this board successfully!");
                continue; // set the error message of this one and continue reading the next one (cause the file is not ended yet.)
            }

            // nextBoard on each step of the loop, will be used to read board data;
            // then the memory it points to will be added as the boards list item,
            // then nextBoard will occupy a new place on memory
            List_add(boards, nextBoard);
        }
    }
    free(nextBoard); // at last step of for, a new empty board is allocated that is not needed

    fclose(boardFile);
    return boards;
}

short saveBoards(List *boards)
{
    // this is used when a board is modified or deleted
    // then the app should remove boards file data and replace its data with the updated data
    // that is stored in boards list
}