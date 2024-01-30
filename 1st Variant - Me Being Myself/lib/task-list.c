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

    if (strlen(title) > MAX_TITLE_LENGTH)
    {
        char err[MAX_RESPONSE_LENGTH] = {'\0'};
        sprintf(err, "Title of a list can not exceed %d characters!", MAX_RESPONSE_LENGTH);
        TaskList_failure(taskList, err);
        return taskList;
    }
    strncpy(taskList->title, title, MAX_TITLE_LENGTH - 1);
    if (now != -1)
    {
        char fileLocation[MAX_FILENAME_LENGTH] = {'\0'};

        taskList->id = (Long)now;
        // the task list created by a user will be tored in a file in task list folder, named by the id of the container board id
        SET_DATA_FILE(fileLocation, FOLDER_LISTS, containerBoard->id); // tasks are saved in Lists folder under the filename of board->id
        if (!fileExists(fileLocation))
        {
            // create the file and add the header row
            FILE *taskListFile = fopen(fileLocation, "w");
            fprintf(taskListFile, "Id%sList Title%sOwner Id\n", COLUMN_DELIMITER, COLUMN_DELIMITER);
            fclose(taskListFile);
        }

        // now add new items
        FILE *taskListFile = fopen(fileLocation, "a");
        if (taskListFile)
        {
            fprintf(taskListFile, "%llu%s\"%s\"%s%llu\n", taskList->id, COLUMN_DELIMITER, taskList->title,
                    COLUMN_DELIMITER, taskList->board->ownerId); // append new task list to file
            fclose(taskListFile);
        }
        else
        {
            TaskList_failure(taskList, "Cannot save List data!");
        }
    }
    else
    {
        TaskList_failure(taskList, "Cannot assign id to this new List!");
    }
    _fcloseall(); // Just to make sure
    return taskList;
}

List *getTaskLists(Board *containerBoard)
{
    // read all the task list from the file
    List *taskLists = newList();
    char taskListsFilename[MAX_FILENAME_LENGTH] = {'\0'};
    SET_DATA_FILE(taskListsFilename, FOLDER_LISTS, containerBoard->id); // now taskListsFile contains the address of the board file that contains desired user board list ata.
    FILE *taskListsFile = fopen(taskListsFilename, "r");
    if (!taskListsFile)
    {
        _fcloseall();
        return taskLists; // user has not create any board; but a empty list will be sent back to caller, so that the user can add items in the future
    }
    TaskList *nextTaskList = newTaskList(); // this is used to read each board data
    string row = String(MAX_LIST_FILE_ROW_LENGTH);
    if (!feof(taskListsFile) && fgets(row, MAX_LIST_FILE_ROW_LENGTH, taskListsFile) != NULL)
    { // the first row are headers(titles); also its a good practice to check that as an insurance that file data is stored correctly.
        for (; !feof(taskListsFile) && fgets(row, MAX_LIST_FILE_ROW_LENGTH, taskListsFile) != NULL; nextTaskList = newTaskList())
        {
            removeNextlineCharacter(row);
            // the second call to fgets will read the credential line
            const string id = (string)strtok(row, COLUMN_DELIMITER);       // split the text by COLUMN_DELIMITER[,] character
            string title = (string)strtok(NULL, COLUMN_DELIMITER);         // get the next column
            const string ownerId = (string)strtok(NULL, COLUMN_DELIMITER); // the ownerId is also in containerBoatd->id
            // this one is used just to check values are correct
            title = trimColumnValue(title);
            if (!id || !title || !ownerId || atol(ownerId) != containerBoard->ownerId)
            {
                if (!feof(taskListsFile)) // if file is not ended and this condition happended, then the data of this board is corrupted
                    TaskList_failure(nextTaskList, "It seems the data related to this list is corrupted or modified!");
                continue;
            }

            // each task list occupies to lines
            // first line is its id and the second is the title
            strncpy(nextTaskList->title, title, MAX_TITLE_LENGTH);
            nextTaskList->id = atol(id); // convert read id to Long
            nextTaskList->board = containerBoard;
            if (!nextTaskList->id)
            {
                TaskList_failure(nextTaskList, "Could not read the id property of this board successfully!");
                continue; // set the error message of this one and continue reading the next one (cause the file is not ended yet.)
            }

            // nextTaskList on each step of the loop, will be used to read list data;
            // then the memory it points to will be added as the taskLists list item,
            // then nextTaskList will occupy a new place on memory
            List_add(taskLists, nextTaskList);
        }
    }
    free(nextTaskList); // at last step of for, a new empty task list is allocated that is not needed

    fclose(taskListsFile);
    return taskLists;
}

short TaskLists_save(List *taskLists, Long containerBoardId)
{
    // this is used when a task list is modified or deleted
    // then the app should remove tasklist file data and replace its data with the updated data
    // that is stored in taskLists list
    char fileLocation[MAX_FILENAME_LENGTH] = {'\0'};
    FILE *taskListFile; // file related to the selected list of a selected board
    TaskList *taskList;
    SET_DATA_FILE(fileLocation, FOLDER_LISTS, containerBoardId);
    // create the file and add the header row
    taskListFile = fopen(fileLocation, "w");
    if (!taskListFile)
    {
        List_failure(taskLists, "Cannot save TaskLists data!");
        return 0; // error happend
    }
    fprintf(taskListFile, "Id%sList Title%sOwner Id\n", COLUMN_DELIMITER, COLUMN_DELIMITER);

    for (Long i = 0; i < taskLists->length; i++)
    {
        taskList = (TaskList *)List_at(taskLists, i);
        fprintf(taskListFile, "%llu%s\"%s\"%s%llu\n", taskList->id, COLUMN_DELIMITER, taskList->title,
                COLUMN_DELIMITER, taskList->board->ownerId);
    }
    fclose(taskListFile);
    return 1;
}

string TaskList_getError(TaskList *taskList)
{
    if (taskList && taskList->error[0])
        // if error string is not empty, return it directly
        return taskList->error;
    if (!taskList)
        return "An UnknownError detected. Trying to restart the app may help resolve or identify the issue ...";

    if (!taskList->id || !taskList->board || !taskList->board->id)
        return "An UnknownError detected and it appears to be related to lists database.\n\tTrying to restart the app may help resolve or identify the issue ...\n";
    return NULL;
}

void TaskList_failure(TaskList *taskList, string msg)
{
    TaskList_reset(taskList);
    sprintf(taskList->error, "Unexpected Behaviour:\t%s\n", msg);
}

void TaskList_print(TaskList *taskList)
{
    printf("Your selected List is as below:\n\n  Id%6s\t\tOwnerId%4s\t\tBoardId%4s\t\tTitle\n", " ", " ", " ");
    PRINT_DASH_ROW();
    printf("%10llu\t\t%10llu\t\t%10llu\t\t%s\n", taskList->id, taskList->board->ownerId, taskList->board->id, taskList->title);
}