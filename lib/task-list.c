#include "task-list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void TaskList_reset(TaskList *taskList)
{
    taskList->title[0] = taskList->location[0] = taskList->error[0] = '\0'; // a no location, becqause the taskList is not saved.
    taskList->id = 0;
    taskList->board = NULL; // identifier of failure
}

TaskList *newTaskList()
{
    TaskList *taskList = (TaskList *)calloc(1, sizeof(TaskList));
    Board_reset(taskList);
    return taskList;
}

TaskList *createTaskList(Board *board, char title[])
{
    TaskList *taskList = newTaskList();
    time_t now = time(NULL);
    taskList->board = board;
    // TODO: is it ok to have same name boards?
    if (strlen(title) > MAX_TITLE_LENGTH)
    {
        char err[MAX_RESPONSE_LENGTH] = {'\0'};
        sprintf(err, "Title of a list can not exceed %d characters!", MAX_TITLE_LENGTH);
        TaskList_failure(taskList, err);
        return board;
    }
    strncpy(taskList->title, title, MAX_TITLE_LENGTH - 1);
    if (now != -1)
    {
        taskList->id = (long)now;
        // the boards created by a user will be tored in a file in Boards folder, named by the id of the board owner

        SET_DATA_FILE(taskList->location, FOLDER_BOARDS, board->id); // tasks are saved in Lists folder under the filename of board->id
        if (!fileExists(taskList->location))
        {
            // creste the file and add the header row
            FILE *taskListFile = fopen(taskList->location, "w");
            fprintf(taskListFile, "Id%sList Title%sBoard Id%sOwner Id\n", COLUMN_DELIMITER, COLUMN_DELIMITER, COLUMN_DELIMITER);
            fclose(taskListFile);
        }
        // now add new items
        FILE *taskListFile = fopen(taskList->location, "a");
        if (taskListFile)
        {
            fprintf(taskListFile, "%ld%s\"%s\"%s%ld%s%ld\n", taskList->id, COLUMN_DELIMITER, taskList->title,
                    COLUMN_DELIMITER, taskList->board->id, taskList->board->ownerId); // append new board to file
            fclose(taskListFile);
        }
        else
        {
            Board_failure(board, "Cannot save List data!");
        }
    }
    else
    {
        Board_failure(board, "Cannot assign id to this new List!");
    }
    _fcloseall(); // Just to make sure
    return taskList;
}

List *getTaskLists(Board *board)
{
}

short TaskLists_save(List *taskLists)
{
}

string TaskList_getError(TaskList *taskList)
{
    return NULL;
}

void TaskList_failure(TaskList *taskList, string msg)
{
    Board_reset(taskList);
    sprintf(taskList->error, "Unexpected Behaviour:\t%s\n", msg);
}