#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void Board_reset(Board *board)
{
    board->title[0] = board->error[0] = '\0'; // a no location, becqause the board is not saved.
    board->id = board->ownerId = 0;           // identifier of failure
}

Board *newBoard()
{
    Board *board = (Board *)calloc(1, sizeof(Board));
    Board_reset(board);
    return board;
}

void Board_failure(Board *board, string msg)
{
    Board_reset(board);
    sprintf(board->error, "Unexpected Behaviour:\t%s\n", msg);
}

Board *createBoard(Long ownerId, char title[])
{
    Board *board = newBoard();
    time_t now = time(NULL);
    board->ownerId = ownerId;

    if (strlen(title) > MAX_TITLE_LENGTH)
    {
        char err[MAX_RESPONSE_LENGTH] = {'\0'};
        sprintf(err, "Title of a board can not exceed %d characters!", MAX_RESPONSE_LENGTH);
        Board_failure(board, err);
        return board;
    }
    strncpy(board->title, title, MAX_TITLE_LENGTH - 1);
    if (now != -1)
    {
        char fileLocation[MAX_FILENAME_LENGTH] = {'\0'};
        board->id = (Long)now;
        // boards created by a user will be stored in a file in Boards folder, named by the id of the board owner
        SET_DATA_FILE(fileLocation, FOLDER_BOARDS, ownerId);
        if (!fileExists(fileLocation))
        {
            // creste the file and add the header row
            FILE *boardFile = fopen(fileLocation, "w");
            fprintf(boardFile, "Id%sBoard Title\n", COLUMN_DELIMITER);
            fclose(boardFile);
        }
        // now add new items
        FILE *boardFile = fopen(fileLocation, "a");
        if (boardFile)
        {
            fprintf(boardFile, "%llu%s\"%s\"\n", board->id, COLUMN_DELIMITER, board->title); // append new board to file
            fclose(boardFile);
        }
        else
        {
            Board_failure(board, "Cannot save Board data!");
        }
    }
    else
    {
        Board_failure(board, "Cannot assign id to this new Board!");
    }
    _fcloseall(); // Just to make sure
    return board;
}

List *getBoards(Long ownerId)
{
    // read all the boards from the file
    List *boards = newList();
    char boardFilename[MAX_FILENAME_LENGTH] = {'\0'};
    SET_DATA_FILE(boardFilename, FOLDER_BOARDS, ownerId); // now boardFile contains the address of the board file that contains desired user board list ata.
    FILE *boardFile = fopen(boardFilename, "r");
    if (!boardFile)
    {
        _fcloseall();
        return boards; // user has not create any board; but a empty list will be sent back to caller, so that the user can add items in the future
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
                    Board_failure(nextBoard, "It seems the data related to this board is corrupted or modified!");
                continue;
            }

            // each board occupies to lines
            // first line is its id and the second is the title
            strncpy(nextBoard->title, title, MAX_TITLE_LENGTH);
            nextBoard->id = atol(id); // convert read id to Long
            nextBoard->ownerId = ownerId;
            if (!nextBoard->id)
            {
                Board_failure(nextBoard, "Could not read the id property of this board successfully!");
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

short Boards_save(List *boards, Long ownerId)
{
    // this is used when a board is modified or deleted
    // then the app should remove boards file data and replace its data with the updated data
    // that is stored in boards list
    char fileLocation[MAX_FILENAME_LENGTH] = {'\0'};
    FILE *userBoardsFile;
    Board *board;
    SET_DATA_FILE(fileLocation, FOLDER_BOARDS, ownerId);
    // creste the file and add the header row
    userBoardsFile = fopen(fileLocation, "w");
    if (!userBoardsFile)
    {
        List_failure(boards, "Cannot save Boards data!");
        return 0; // error happend
    }

    fprintf(userBoardsFile, "Id%sBoard Title\n", COLUMN_DELIMITER);
    for (Long i = 0; i < boards->length; i++)
    {
        board = (Board *)List_at(boards, i);
        fprintf(userBoardsFile, "%llu%s\"%s\"\n", board->id, COLUMN_DELIMITER, board->title); // save all boards by board
    }
    fclose(userBoardsFile);
    return 1;
}

string Board_getError(Board *board)
{

    if (board && board->error[0])
        // if error string is not empty, return it directly
        return board->error;
    if (!board)
        return "An UnknownError detected. Trying to restart the app may help resolve or identify the issue ...";

    if (!board->id)
        return "An UnknownError detected and it appears to be related to boards database.\n\tTrying to restart the app may help resolve or identify the issue ...\n";
    return NULL;
}

void Board_print(Board *board)
{
    printf("Your selected board is as below:\n\n  Id%6s\t\tOwnerId%4s\t\tTitle\n", " ", " ");
    PRINT_DASH_ROW();
    printf("%10llu\t\t%10llu\t\t%s\n", board->id, board->ownerId, board->title);
}

short Board_delete(Board *board)
{

}