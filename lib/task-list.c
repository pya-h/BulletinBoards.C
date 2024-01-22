#include "task-list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void TaskList_reset(TaskList *taskList)
{
    taskList->title[0] = taskList->error[0] = '\0'; // a no location, becqause the taskList is not saved.
    taskList->id = 0;
    taskList->board = NULL; // identifier of failure
}

TaskList *newTaskList()
{
    TaskList *taskList = (TaskList *)calloc(1, sizeof(TaskList));
    TaskList_reset(taskList);
    return taskList;
}

TaskList *createTaskList(Board *containerBoard, char title[])
{
    TaskList *taskList = newTaskList();
    time_t now = time(NULL);
    taskList->board = containerBoard;
    // TODO: is it ok to have same name boards?
    if (strlen(title) > MAX_TITLE_LENGTH)
    {
        char err[MAX_RESPONSE_LENGTH] = {'\0'};
        sprintf(err, "Title of a list can not exceed %d characters!", MAX_TITLE_LENGTH);
        TaskList_failure(taskList, err);
        return taskList;
    }
    strncpy(taskList->title, title, MAX_TITLE_LENGTH - 1);
    printf("stage 1 done!\n");
    if (now != -1)
    {
        char fileLocation[MAX_FILENAME_LENGTH] = {'\0'};

        taskList->id = (long)now;
        // the boards created by a user will be tored in a file in Boards folder, named by the id of the board owner
        SET_DATA_FILE(fileLocation, FOLDER_LISTS, containerBoard->id); // tasks are saved in Lists folder under the filename of board->id
        if (!fileExists(fileLocation))
        {
            // creste the file and add the header row
            FILE *taskListFile = fopen(fileLocation, "w");
            fprintf(taskListFile, "Id%sList Title%sBoard Id%sOwner Id\n", COLUMN_DELIMITER, COLUMN_DELIMITER, COLUMN_DELIMITER);
            fclose(taskListFile);
        }
    printf("stage 2 done!\n");

        // now add new items
        FILE *taskListFile = fopen(fileLocation, "a");
        if (taskListFile)
        {
            printf("stage 3 done!\n");
            fprintf(taskListFile, "%ld%s\"%s\"%s%ld%s%ld\n", taskList->id, COLUMN_DELIMITER, taskList->title,
                    COLUMN_DELIMITER, taskList->board->id, COLUMN_DELIMITER, taskList->board->ownerId); // append new board to file
            fclose(taskListFile);
            printf("stage 4 done!\n");
        }
        else
        {
            printf("stage 1 error!\n");
            TaskList_failure(taskList, "Cannot save List data!");
            printf("stage 1 error done!\n");
        }
    }
    else
    {
        printf("stage 2 error!\n");
        TaskList_failure(taskList, "Cannot assign id to this new List!");
        printf("stage 2 error done!\n");
    }
    _fcloseall(); // Just to make sure
    return taskList;
}

List *getTaskLists(Board *containerBoard)
{
}

short TaskLists_save(List *taskLists)
{
}

string TaskList_getError(TaskList *taskList)
{
    if(taskList && taskList->error[0])
        // if error string is not empty, return it directly
        return taskList->error;
    if(!taskList)
        return "An UnknownError detected. Trying to restart the app may help resolve or identify the issue ...";
    
    if(!taskList->id || !taskList->board || !taskList->board->id)
        return "An UnknownError detected and it appears to be related to lists database.\n\tTrying to restart the app may help resolve or identify the issue ...\n";
    return NULL;
}

void TaskList_failure(TaskList *taskList, string msg)
{
    TaskList_reset(taskList);
    sprintf(taskList->error, "Unexpected Behaviour:\t%s\n", msg);
}